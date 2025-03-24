#pragma once

#include<array>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include<glm/glm.hpp>

#include <nlohmann/json.hpp>
#include <tileson.hpp>

#include "util.h"
#include "tile_world.h"


class Map
{
    public:
        Map();
        ~Map();

        void Draw(sf::RenderWindow &cWindow, TileWorld &cTileWorld);

        void LoadFromFile(const std::string &_sFilepath);

    private:
        sf::Sprite* storeAndLoadImage(const std::string &_image, const sf::Vector2f &_vPos);

    public:
        std::unique_ptr<tson::Map> pMap;
        std::unique_ptr<sf::Sprite> pSprite;

    private:
        std::map<std::string, std::unique_ptr<sf::Texture>> m_pTextures;
        std::map<std::string, std::unique_ptr<sf::Sprite>> m_pSprites;

        glm::vec2 m_vDefaultScalar = glm::vec2(1.0f);

        fs::path m_basePath;
};
