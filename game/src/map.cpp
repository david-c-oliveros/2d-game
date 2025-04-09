#include "map.h"



Map::Map()
{
    m_basePath = fs::path("../../res/tilemaps/rpg_world_pack_demo");
}



Map::~Map()
{
}



void Map::Draw(sf::RenderWindow &cWindow, const glm::ivec2 &_vWorldGridPos)
{
    /*********************************************************/
    /*        Get Adjacent tiles to adjust highlight         */
    /*        depending on how many tiles are stacked        */
    /*********************************************************/
    std::map<std::string, uint32_t> mDupeCount;
    std::vector<std::shared_ptr<Tile>> aAdj = GetAdjacentTiles(_vWorldGridPos);

    for (auto &tile : aAdj)
    {
        if (mDupeCount.count(glm::to_string(tile->vWorldGridPos)) > 0)
        {
            mDupeCount.at(glm::to_string(tile->vWorldGridPos))++;
        }
        else
        {
            mDupeCount[glm::to_string(tile->vWorldGridPos)] = 0;
        }
    }

    /****************************/
    /*        Draw tiles        */
    /****************************/
    for (auto &tile : aTiles)
    {
        if (tile->pSprite == nullptr)
        {
            continue;
        }

        tile->pSprite->setTextureRect(sf::Rect( sf::Vector2i(tile->cTextureRect.x, tile->cTextureRect.y),
                                                sf::Vector2i(tile->cTextureRect.width, tile->cTextureRect.height)) );

        glm::ivec2 _vSpritePos = tile->vWorldGridPos * Util::convert_vector<glm::ivec2>(Globals::TILE_SIZE);
        tile->pSprite->setPosition(Util::convert_vector<sf::Vector2f>(_vSpritePos));

        cWindow.draw(*tile->pSprite);


        /*****************************************************/
        /*        DEBUG: Highlight certain tile types        */
        /*               and adjacent tiles                  */
        /*****************************************************/
        /*********************************/
        /*        DEBUG Level One        */
        /*********************************/
        if (Globals::eDEBUG_LEVEL == Globals::DebugLevel::ZERO)
            continue;

        if (tile->bSolid)
        {
            sf::FloatRect rect = GetTileBox(*tile);
            sf::RectangleShape shape(rect.size);
            shape.setPosition(rect.position);
            shape.setFillColor(sf::Color(100, 50, 0, 180));
            cWindow.draw(shape);
        }

        /*********************************/
        /*        DEBUG Level Two        */
        /*********************************/
        if (Globals::eDEBUG_LEVEL == Globals::DebugLevel::ONE)
            continue;

        if (mDupeCount.count(glm::to_string(tile->vWorldGridPos)) > 0)
        {
            uint32_t num = 40 * mDupeCount.at(glm::to_string(tile->vWorldGridPos));
            tile->pSprite->setColor(sf::Color(150 - num, 150 - num, 150 - num));
        }

        cWindow.draw(*tile->pSprite);
    }
}



//void Map::DrawNavTiles()
//{
//}



std::vector<std::shared_ptr<Tile>> Map::GetCurrentTiles(const glm::ivec2 &_vWorldGridPos)
{
    std::vector<std::shared_ptr<Tile>> aCurrentTiles;
    auto isOnTile = [_vWorldGridPos](std::shared_ptr<Tile> t) { return t->vWorldGridPos.x == _vWorldGridPos.x &&
                                                                       t->vWorldGridPos.y == _vWorldGridPos.y;
    };

    std::vector<std::shared_ptr<Tile>>::iterator it = std::find_if(aTiles.begin(), aTiles.end(), isOnTile);
    while(it != aTiles.end())
    {
        aCurrentTiles.emplace_back(*it);
        ++it;
        it = std::find_if(++it, aTiles.end(), isOnTile);
    }

    return aCurrentTiles;
}



std::vector<std::shared_ptr<Tile>> Map::GetAdjacentTiles(const glm::ivec2 &_vWorldGridPos)
{
    std::vector<std::shared_ptr<Tile>> aAdjacentTiles;
    auto isAdjacent = [_vWorldGridPos](std::shared_ptr<Tile> t) { return t->vWorldGridPos.x <= _vWorldGridPos.x + 1 &&
                                                                         t->vWorldGridPos.x >= _vWorldGridPos.x - 1 &&
                                                                         t->vWorldGridPos.y <= _vWorldGridPos.y + 1 &&
                                                                         t->vWorldGridPos.y >= _vWorldGridPos.y - 1 &&
                                                                         t->vWorldGridPos != _vWorldGridPos;
    };

    std::vector<std::shared_ptr<Tile>>::iterator it = std::find_if(aTiles.begin(), aTiles.end(), isAdjacent);
    while(it != aTiles.end())
    {
        aAdjacentTiles.emplace_back(*it);
        it = std::find_if(++it, aTiles.end(), isAdjacent);
    }

    return aAdjacentTiles;
}



void Map::LoadFromFile(const std::string &_sFilepathStr)
{
    /* Tileson uses an alias 'fs' for 'std::filesystem' */
    tson::Tileson t;
    fs::path fp(m_basePath / _sFilepathStr);

    pTsonMap = t.parse(fp);

    if (pTsonMap->getStatus() == tson::ParseStatus::OK)
    {
        tson::Vector2i _vTileSize = pTsonMap->getTileSize();
        m_vDefaultScalar /= glm::vec2(_vTileSize.x, _vTileSize.y);
        aTiles.reserve(pTsonMap->getSize().x * pTsonMap->getSize().y);
        std::cout << "Map parsed successfully" << std::endl;
    }
    else
    {
        switch(pTsonMap->getStatus())
        {
            case tson::ParseStatus::FileNotFound:
                std::cout << "File not found" << std::endl;
                break;
            case tson::ParseStatus::ParseError:
                std::cout << "Parse error" << std::endl;
                break;
            case tson::ParseStatus::MissingData:
                std::cout << "Missing data" << std::endl;
                break;
            case tson::ParseStatus::DecompressionError:
                std::cout << "DecompressionError" << std::endl;
                break;
            default:
                std::cout << "Other error" << std::endl;
                break;
        }
    }

    storeMap();
}



void Map::storeMap()
{
    uint32_t nLayer = 0;
    for (auto &_layer : pTsonMap->getLayers())
    {
        if (_layer.getType() == tson::LayerType::TileLayer)
        {
            int current_layer_id = _layer.getId();
            std::cout << "Current layer ID: " << current_layer_id << '\n';
            std::cout << "Layer size: " << glm::to_string(Util::convert_vector<glm::ivec2>(_layer.getSize())) << '\n';

            /*******************************************************/
            /*         Initialize size of Nav Tiles array          */
            /*        But only do this once (on first layer)       */
            /*******************************************************/
            if (nLayer == 0)
            {
                vMapSize = glm::ivec2(_layer.getSize().x, _layer.getSize().y);
                aNavTiles.resize((size_t)(vMapSize.x * vMapSize.y), NavTile{ glm::ivec2(0.0f), false });
            }
            nLayer++;


            for (auto &[pos, tileObject] : _layer.getTileObjects())
            {
                /*******************************/
                /*        Get Tile data        */
                /*******************************/
                // TODO - If new layer tile occupies a full tile, overwrite the old tile
                std::shared_ptr<Tile> _pTile = std::make_shared<Tile>();

                _pTile->tileset = tileObject.getTile()->getTileset();
                _pTile->cTextureRect = tileObject.getDrawingRect();

                tson::Vector2f _vPos = tileObject.getPosition();
                _pTile->vWorldGridPos = glm::ivec2(_vPos.x, _vPos.y) / Util::convert_vector<glm::ivec2>(Globals::TILE_SIZE);


                _pTile->pSprite = storeAndLoadImage(_pTile->tileset->getImage().u8string(), {0, 0});

                /********************************************/
                /*        Set whether layer is solid        */
                /********************************************/
                // TODO - Make enum that contains ID's of all solid tile types
                if (current_layer_id == 2)
                {
                    _pTile->bSolid = true;
                }
                else
                {
                    _pTile->bSolid = false;
                }

                storeNavTile(_pTile);

                if (Globals::eDEBUG_LEVEL < Globals::DebugLevel::TWO)
                {
                  aTiles.emplace_back(_pTile);
                  continue;
                }


                /*******************************************/
                /*        DEBUG: Skip stacked tiles        */
                /*******************************************/
                std::vector<std::shared_ptr<Tile>>::iterator it = getOccupiedTile(_pTile->vWorldGridPos);

                if (it != aTiles.end())
                {
                    *it = _pTile;
                }
                else
                {
                  aTiles.emplace_back(_pTile);
                }
            }
        }
    }
}



void Map::storeNavTile(std::shared_ptr<Tile> tile)
{
    /************************************************************/
    /************************************************************/
    /*         Create new NavTile from tile if the              */
    /*         NavTile at the current index is a nullptr        */
    /*                                                          */
    /*         Otherwise overwrite only if the current          */
    /*                    tile is solid                         */
    /************************************************************/
    /************************************************************/
    int nIndex = tile->vWorldGridPos.y * vMapSize.x + tile->vWorldGridPos.x;;

    if (!aNavTiles.at(nIndex).bOccupied)
    {
        aNavTiles.at(nIndex).vWorldGridPos = tile->vWorldGridPos;
        aNavTiles.at(nIndex).bSolid = tile->bSolid;
        aNavTiles.at(nIndex).bOccupied = true;
        return;
    }

    if (tile->bSolid)
    {
        aNavTiles.at(nIndex).vWorldGridPos = tile->vWorldGridPos;
        aNavTiles.at(nIndex).bSolid = tile->bSolid;
    }
}



sf::Sprite* Map::storeAndLoadImage(const std::string &image, const sf::Vector2f &_vPos)
{
    if (m_pTextures.count(image) == 0)
    {
        fs::path path = m_basePath / image;

        if (fs::exists(path) && fs::is_regular_file(path))
        {
            std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
            bool bImageFound = tex->loadFromFile(path.generic_string());
            tex->setSmooth(false);

            if (bImageFound)
            {
                std::unique_ptr<sf::Sprite> spr = std::make_unique<sf::Sprite>(*tex);
//                spr->setTexture(*tex);
                spr->setPosition(_vPos);
                m_pTextures[image] = std::move(tex);
                m_pSprites[image] = std::move(spr);
            }
        }
        else
        {
            std::cout << "Could not find: " << path.generic_string() << std::endl;
        }
    }

    if (m_pSprites.count(image) > 0)
    {
        return m_pSprites[image].get();
    }

    return nullptr;
}



std::vector<std::shared_ptr<Tile>>::iterator Map::getOccupiedTile(glm::ivec2 _vWorldGridPos)
{
    auto vecEqual = [_vWorldGridPos](std::shared_ptr<Tile> t) { return t->vWorldGridPos.x == _vWorldGridPos.x &&
                                                                   t->vWorldGridPos.y == _vWorldGridPos.y;
    };

    return std::find_if(aTiles.begin(), aTiles.end(), vecEqual);
}




/**********************************/
/**********************************/
/*                                */
/*        Static functions        */
/*                                */
/**********************************/
/**********************************/
sf::FloatRect Map::GetTileBox(Tile &tile)
{
    sf::Vector2f _vTileWorldPos = Util::convert_vector<sf::Vector2f>((glm::vec2)tile.vWorldGridPos * Globals::GLM_TILE_SIZE);
    return sf::FloatRect(_vTileWorldPos, Globals::TILE_SIZE);
}
