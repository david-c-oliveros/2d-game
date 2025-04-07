#pragma once

#include<array>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include<glm/glm.hpp>

#include <nlohmann/json.hpp>
#include <tileson.hpp>

#include "util.h"
#include "tile_world.h"



struct Tile
{
    tson::Tileset *tileset;
    tson::Rect cTextureRect;
    glm::vec2 vWorldPos;
    sf::Sprite *pSprite;
};


class Map
{
    public:
        Map();
        ~Map();

        void Draw(sf::RenderWindow &cWindow);
        void LoadFromFile(const std::string &_sFilepath);

    private:
        void storeMap();
        sf::Sprite* storeAndLoadImage(const std::string &_image, const sf::Vector2f &_vPos);

    public:
        std::unique_ptr<tson::Map> pMap;
        std::unique_ptr<sf::Sprite> pSprite;
        //std::array<std::unique_ptr<Tile>, 10000> aTiles = { nullptr };
        std::vector<std::unique_ptr<Tile>> aTiles;

    private:
        std::map<std::string, std::unique_ptr<sf::Texture>> m_pTextures;
        std::map<std::string, std::unique_ptr<sf::Sprite>> m_pSprites;

        glm::vec2 m_vDefaultScalar = glm::vec2(1.0f);

        fs::path m_basePath;
};
