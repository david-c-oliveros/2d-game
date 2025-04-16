#include "map.h"



Map::Map()
{
    m_basePath = fs::path("../../res/tilemaps/rpg_world_pack_demo");
}



Map::~Map()
{
}



void Map::Draw(const glm::ivec2 &_vWorldGridPos, const GLShader &cShader)
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

        tile->pSprite->SetTextureRect(sf::Rect( sf::Vector2i(tile->cTextureRect.x, tile->cTextureRect.y),
                                                sf::Vector2i(tile->cTextureRect.width, tile->cTextureRect.height)) );

        glm::vec2 vSpritePos = (glm::vec2)tile->vWorldGridPos * Globals::GLM_TILE_SIZE;
        tile->pSprite->SetPosition(vSpritePos);

        SpriteRenderer::Draw(*tile->pSprite, "map_shader");
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
    }
    else
    {
        switch(pTsonMap->getStatus())
        {
            case tson::ParseStatus::FileNotFound:
                util::Log("ERROR::TILESON::File not found");
                break;
            case tson::ParseStatus::ParseError:
                util::Log("ERROR::TILESON::Parse error");
                break;
            case tson::ParseStatus::MissingData:
                util::Log("ERROR::TILESON::Missing data");
                break;
            case tson::ParseStatus::DecompressionError:
                util::Log("ERROR::TILESON::Decompression error");
                break;
            default:
                util::Log("ERROR::TILESON::Other error");
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
                _pTile->vWorldGridPos = glm::ivec2(_vPos.x, _vPos.y) / util::convert_vector<glm::ivec2>(Globals::TILE_SIZE);


                _pTile->pSprite = storeAndLoadImage(_pTile->tileset->getImage(), {0, 0});
                // TODO - Look into why this doesn't work in C++20
//                _pTile->pSprite = storeAndLoadImage(_pTile->tileset->getImage().u8string(), {0, 0});

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



std::shared_ptr<GLSprite> Map::storeAndLoadImage(const std::string &sImageName, const glm::vec2 &_vPos)
{
    if (0 == m_pSprites.count(sImageName))
    {
        fs::path path = m_basePath / sImageName;

        if (fs::exists(path) && fs::is_regular_file(path))
        {
            // TODO - Properly pass in bAlpha
            RM::LoadTexture(path.generic_string(), true, sImageName);
            std::shared_ptr<GLSprite> spr = std::make_shared<GLSprite>(sImageName);
            spr->SetPosition(_vPos);
            spr->SetColor(glm::vec4(1.0f));
            m_pSprites[sImageName] = std::move(spr);
        }
        else
        {
            util::Log("ERROR::MAP::Could not find image: ", false);
            util::Log(path.generic_string());
        }
    }

    if (0 < m_pSprites.count(sImageName))
    {
        return m_pSprites[sImageName];
    }

    return m_pSprites[sImageName];
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
    sf::Vector2f _vTileWorldPos = util::convert_vector<sf::Vector2f>((glm::vec2)tile.vWorldGridPos * Globals::GLM_TILE_SIZE);
    return sf::FloatRect(_vTileWorldPos, Globals::TILE_SIZE);
}
