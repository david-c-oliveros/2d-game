#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include <glm/glm.hpp>

#include "tile_world.h"



class Entity
{
    public:
        Entity(glm::vec2 _vWorldPos);
        ~Entity();

        void Create();
        virtual void Update() = 0;
        virtual void Draw(sf::RenderWindow &cWindow) = 0;


    public:
        glm::vec2 vWorldPos;
};
