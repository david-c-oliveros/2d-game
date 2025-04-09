#pragma once

#include <iostream>
#include <limits>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SFML/Graphics.hpp>

#include "map.h"



struct Hit
{
    float fTime;
    float fRemainingTime;
    glm::vec2 vNormal;
};



struct Circle
{
    glm::vec2 vPos;
    float fRadius;
};



class Collision
{
    public:
        static bool AABB(sf::FloatRect boxA, sf::FloatRect boxB);
        static Hit SweptAABB(sf::FloatRect boxA, sf::FloatRect boxB, glm::vec2 velA, glm::vec2 velB);
        static glm::vec2 SweepResponse(Hit sweepResult, glm::vec2 vEntityVel);
        static glm::vec2 CircleSquare(Circle c, glm::vec2 vVel, const std::vector<NavTile> &aTiles, glm::ivec2 vWorldSize);


    private:
        static glm::vec2 sweepResponseSlide(Hit sweepResult, glm::vec2 vEntityVel);
};
