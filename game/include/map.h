#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <tileson.hpp>

#include "globals.h"
#include "util.h"
#include "shader.h"
#include "texture.h"
#include "sprite.h"
#include "renderer.h"
#include "sprite_renderer.h"



struct Tile
{
    tson::Tileset *tileset;
    tson::Rect cTextureRect;
    glm::ivec2 vWorldGridPos;
    std::shared_ptr<GLSprite> pSprite;
    bool bSolid;
    bool bCollided;
};



struct NavTile
{
    glm::ivec2 vWorldGridPos;
    bool bSolid;
    bool bOccupied = false;
};


class Map
{
    public:
        Map();
        ~Map();

        void Draw(const glm::ivec2 &_vWorldGridPos, const GLShader &cShader);
        std::vector<std::shared_ptr<Tile>> GetCurrentTiles(const glm::ivec2 &_vWorldGridPos);
        std::vector<std::shared_ptr<Tile>> GetAdjacentTiles(const glm::ivec2 &_vWorldGridPos);
        void LoadFromFile(const std::string &_sFilepath);

    private:
        void storeMap();
        void storeNavTile(std::shared_ptr<Tile> tile);
        std::shared_ptr<GLSprite> storeAndLoadImage(const std::string &sImageName, const glm::vec2 &_vPos);
        std::vector<std::shared_ptr<Tile>>::iterator getOccupiedTile(glm::ivec2 _vWorldPos);

    public:
        std::unique_ptr<tson::Map> pTsonMap;
        std::vector<std::shared_ptr<Tile>> aTiles;
        std::vector<NavTile> aNavTiles;
        glm::ivec2 vMapSize;

    private:
        std::map<std::string, std::shared_ptr<GLSprite>> m_pSprites;

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
        static sf::FloatRect GetTileBox(Tile &tile);
};
