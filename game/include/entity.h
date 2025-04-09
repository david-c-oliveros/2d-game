#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "tile_world.h"
#include "map.h"



class Entity
{
    public:
        Entity(uint32_t _ID, glm::vec2 _vWorldPos);
        ~Entity();

        void Create();
        virtual void Update(Map &cMap) = 0;
        virtual void Draw(sf::RenderWindow &cWindow) = 0;


    public:
        glm::vec2 vWorldPos;
        uint32_t m_ID;
};
