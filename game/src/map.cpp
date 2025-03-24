#include "map.h"



Map::Map()
{
    m_basePath = fs::path("../../res/tilemaps/rpg_world_pack_demo");
}



Map::~Map()
{
}



void Map::Draw(sf::RenderWindow &cWindow, TileWorld &cTileWorld)
{
    for (auto &_layer : pMap->getLayers())
    {
        if (_layer.getType() == tson::LayerType::TileLayer)
        {
            for (auto &[pos, tileObject] : _layer.getTileObjects())
            {
                tson::Tileset *tileset = tileObject.getTile()->getTileset();
                tson::Rect drawingRect = tileObject.getDrawingRect();
                tson::Vector2f _vTilePos = tileObject.getPosition();

                /**************************************************/
                /*        Convert to TileWorld coordinates        */
                /**************************************************/
                glm::vec2 _vWorldPos = glm::vec2(_vTilePos.x, _vTilePos.y) * m_vDefaultScalar;
                glm::vec2 _vScalar = cTileWorld.GetWorldScale();
                glm::vec2 _vScreenPos = cTileWorld.WorldToScreen(_vWorldPos);

                sf::Sprite* sprite = storeAndLoadImage(tileset->getImage().u8string(), {0, 0});

                if (sprite != nullptr)
                {
                    sprite->setTextureRect(sf::Rect( sf::Vector2i(drawingRect.x, drawingRect.y),
                                                       sf::Vector2i(drawingRect.width, drawingRect.height)) );

//                    sprite->setPosition({ _vTilePos.x, _vTilePos.y});
                    sprite->setPosition(Util::glm_to_sf_vec2(_vScreenPos));
                    sprite->setScale(Util::glm_to_sf_vec2(_vScalar * m_vDefaultScalar));
                    cWindow.draw(*sprite);
                }
            }
        }
    }
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
