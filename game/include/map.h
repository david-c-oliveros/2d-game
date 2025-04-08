#pragma once

#include <array>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <nlohmann/json.hpp>
#include <tileson.hpp>

#include "globals.h"
#include "util.h"
#include "tile_world.h"



struct Tile
{
    tson::Tileset *tileset;
    tson::Rect cTextureRect;
    glm::ivec2 vWorldGridPos;
    sf::Sprite *pSprite;
    bool bSolid;
    bool bCollided;
};


class Map
{
    public:
        Map();
        ~Map();

        void Draw(sf::RenderWindow &cWindow, const glm::ivec2 &_vWorldGridPos);
        void LoadFromFile(const std::string &_sFilepath);
        std::vector<std::shared_ptr<Tile>> GetCurrentTiles(const glm::ivec2 &_vWorldGridPos);
        std::vector<std::shared_ptr<Tile>> GetAdjacentTiles(const glm::ivec2 &_vWorldGridPos);

    private:
        void storeMap();
        sf::Sprite* storeAndLoadImage(const std::string &_image, const sf::Vector2f &_vPos);
        std::vector<std::shared_ptr<Tile>>::iterator getOccupiedTile(glm::ivec2 _vWorldPos);

    public:
        std::unique_ptr<tson::Map> pMap;
        std::unique_ptr<sf::Sprite> pSprite;
        std::vector<std::shared_ptr<Tile>> aTiles;

    private:
        std::map<std::string, std::unique_ptr<sf::Texture>> m_pTextures;
        std::map<std::string, std::unique_ptr<sf::Sprite>> m_pSprites;

        glm::vec2 m_vDefaultScalar = glm::vec2(1.0f);

        fs::path m_basePath;


    /**********************************/
    /**********************************/
    /*                                */
    /*        Static functions        */
    /*                                */
    /**********************************/
    /**********************************/
    public:
        static sf::FloatRect GetTileBoundingBox(Tile &tile);
};
