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
    /*        Get Adjacent tiles and adjust highlight        */
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

        if (tile->bSolid)
        {
            tile->pSprite->setColor(sf::Color(180, 0, 0));
        }
        else
        {
            tile->pSprite->setColor(sf::Color(255, 255, 255));
        }

        if (mDupeCount.count(glm::to_string(tile->vWorldGridPos)) > 0)
        {
            uint32_t num = 40 * mDupeCount.at(glm::to_string(tile->vWorldGridPos));
            tile->pSprite->setColor(sf::Color(155 - num, 155 - num, 200 - num));
        }

        cWindow.draw(*tile->pSprite);
    }
}



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
                                                                         t->vWorldGridPos.y >= _vWorldGridPos.y - 1;
    };

    std::vector<std::shared_ptr<Tile>>::iterator it = std::find_if(aTiles.begin(), aTiles.end(), isAdjacent);

    while(it != aTiles.end())
    {
        aAdjacentTiles.emplace_back(*it);
        ++it;
        it = std::find_if(++it, aTiles.end(), isAdjacent);
    }

    return aAdjacentTiles;
}



void Map::LoadFromFile(const std::string &_sFilepathStr)
{
    /* Tileson uses an alias 'fs' for 'std::filesystem' */
    tson::Tileson t;
    fs::path fp(m_basePath / _sFilepathStr);

    pMap = t.parse(fp);

    if (pMap->getStatus() == tson::ParseStatus::OK)
    {
        tson::Vector2i _vTileSize = pMap->getTileSize();
        m_vDefaultScalar /= glm::vec2(_vTileSize.x, _vTileSize.y);
        aTiles.reserve(pMap->getSize().x * pMap->getSize().y);
        std::cout << "Map parsed successfully" << std::endl;
    }
    else
    {
        switch(pMap->getStatus())
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
    for (auto &_layer : pMap->getLayers())
    {
        if (_layer.getType() == tson::LayerType::TileLayer)
        {
            int current_layer_id = _layer.getId();
            std::cout << "Current layer ID: " << current_layer_id << '\n';

            for (auto &[pos, tileObject] : _layer.getTileObjects())
            {
                /*******************************/
                /*        Get Tile data        */
                /*******************************/
                // TODO - If new layer tile occupies a full tile, overwrite the old tile
                std::unique_ptr<Tile> _pTile = std::make_unique<Tile>();

                //std::cout << tileObject.getTile()->getId() << ' ';
                _pTile->tileset = tileObject.getTile()->getTileset();
                _pTile->cTextureRect = tileObject.getDrawingRect();

                tson::Vector2f _vPos = tileObject.getPosition();
                _pTile->vWorldGridPos = glm::ivec2(_vPos.x, _vPos.y) / Util::convert_vector<glm::ivec2>(Globals::TILE_SIZE);
                std::cout << glm::to_string(_pTile->vWorldGridPos) << ' ';


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

//                std::vector<std::shared_ptr<Tile>>::iterator it = getOccupiedTile(_pTile->vWorldPos);
//
//                if (it != aTiles.end())
//                {
//                    *it = std::move(_pTile);
//                }
//                else
//                {
                    aTiles.emplace_back(std::move(_pTile));
//                }
            }
        }
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
