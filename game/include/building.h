#pragma once

#include <memory>
#include <SFML/System/Vector2.hpp>
#include <glm/glm.hpp>

#include "globals.h"
#include "entity.h"


class Map;
class GLSprite;



class Building : public Entity
{
    public:
        Building() = default;
        Building(uint32_t _ID, const glm::vec2 &_vWorldPos);
        Building(uint32_t _ID, std::string _sName, const glm::vec2 &_vWorldPos);
        Building(Building&& other);
        ~Building();

        void Update(Map &cMap);
        void Draw(const std::string sShader);
        void DrawDebug(const std::string sShader);
        void AttachSprite(const std::string _sSpriteName,
                          const std::string sFilepath,
                          glm::ivec2 _vSpriteSize,
                          glm::ivec2 _vFrameRect);


    public:
        std::string sName;


    private:
        glm::vec2 m_vScale = Globals::GLM_TILE_SIZE;
        sf::Vector2i m_vSpriteSize;
        std::unique_ptr<GLSprite> m_pSprite;
};
