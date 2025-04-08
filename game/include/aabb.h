#pragma once

#include <SFML/Graphics.hpp>



class AABB
{
    public:
        static bool CheckCollision(sf::FloatRect boxA, sf::FloatRect boxB);
};
