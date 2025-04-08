#include "aabb.h"




bool AABB::CheckCollision(sf::FloatRect boxA, sf::FloatRect boxB)
{
    bool colX =  boxA.position.x + boxA.size.x >= boxB.position.x &&
                 boxB.position.x + boxB.size.x >= boxA.position.x;

    bool colY =  boxA.position.y + boxA.size.y >= boxB.position.y &&
                 boxB.position.y + boxB.size.y >= boxA.position.y;

    return colX && colY;
}
