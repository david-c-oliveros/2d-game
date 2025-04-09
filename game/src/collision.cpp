#include "collision.h"




bool Collision::AABB(sf::FloatRect boxA, sf::FloatRect boxB)
{
    bool colX =  boxA.position.x + boxA.size.x >= boxB.position.x &&
                 boxB.position.x + boxB.size.x >= boxA.position.x;

    bool colY =  boxA.position.y + boxA.size.y >= boxB.position.y &&
                 boxB.position.y + boxB.size.y >= boxA.position.y;

    return colX && colY;
}



Hit Collision::SweptAABB(sf::FloatRect boxA, sf::FloatRect boxB, glm::vec2 velA, glm::vec2 velB)
{
    glm::vec2 vNormal;
    glm::vec2 vInvEntry;
    glm::vec2 vInvExit;

    if (velA.x > 0.0f)
    {
        vInvEntry.x = boxB.position.x - (boxA.position.x + boxA.size.x);
        vInvExit.x = (boxB.position.x + boxB.size.x) - boxA.position.x;
    }
    else
    {
        vInvEntry.x = (boxB.position.x + boxB.size.x) - boxA.position.x;
        vInvExit.x = boxB.position.x - (boxA.position.x, + boxA.size.x);
    }

    if (velA.y > 0.0f)
    {
        vInvEntry.y = boxB.position.y - (boxA.position.y + boxA.size.y);
        vInvExit.y = (boxB.position.y + boxB.size.y) - boxA.position.y;
    }
    else
    {
        vInvEntry.y = (boxB.position.y + boxB.size.y) - boxA.position.y;
        vInvExit.y = boxB.position.y - (boxA.position.y + boxA.size.y);
    }


    glm::vec2 vEntry;
    glm::vec2 vExit;

    if (velA.x == 0.0f)
    {
        vEntry.x = -std::numeric_limits<float>::infinity();
        vExit.x = std::numeric_limits<float>::infinity();
    }
    else
    {
        vEntry.x = vInvEntry.x / velA.x;
        vExit.x = vInvExit.x / velA.x;
    }

    if (velA.y == 0.0f)
    {
        vEntry.y = -std::numeric_limits<float>::infinity();
        vExit.y = std::numeric_limits<float>::infinity();
    }
    else
    {
        vEntry.y = vInvEntry.y / velA.y;
        vExit.y = vInvExit.y / velA.y;
    }

    float fEntryTime = std::max(vEntry.x, vEntry.y);
    float fExitTime = std::min(vExit.x, vExit.y);

    if (fEntryTime > fExitTime || vEntry.x < 0.0f && vEntry.y < 0.0f || vEntry.x > 1.0f || vEntry.y > 1.0f)
    {
        vNormal.x = 0.0f;
        vNormal.y = 0.0f;
        return Hit{ 1.0f, 0.0f, vNormal };
    }
    else
    {
        if (vEntry.x > vEntry.y)
        {
            if (vInvEntry.x < 0.0f)
            {
                vNormal.x = 1.0f;
                vNormal.y = 0.0f;
            }
            else
            {
                vNormal.x = -1.0f;
                vNormal.y = 0.0f;
            }
        }
        else
        {
            if (vInvEntry.y < 0.0f)
            {
                vNormal.x = 0.0f;
                vNormal.y = 1.0f;
            }
            else
            {
                vNormal.x = 0.0f;
                vNormal.y = -1.0f;
            }
        }
    }

    Hit hit;
    hit.vNormal = vNormal;
    hit.fTime = fEntryTime;
    hit.fRemainingTime = 1.0f - hit.fTime;

    return hit;
}



glm::vec2 Collision::SweepResponse(Hit sweepResult, glm::vec2 vEntityVel)
{
    if (sweepResult.fTime < 1.0f)
    {
        std::cout << "time: " << std::to_string(sweepResult.fTime) << '\n';
        glm::vec2 vCorrectVel = glm::vec2(vEntityVel.x * sweepResult.fTime,
                                vEntityVel.y * sweepResult.fTime);

        glm::vec2 vResponseVel = sweepResponseSlide(sweepResult, vEntityVel);

        glm::vec2 vNewVel(vCorrectVel + vResponseVel);

//        std::cout << "New velocity: " << glm::to_string(vNewVel) << '\n';

        return vNewVel;
    }

    return vEntityVel;
}



glm::vec2 Collision::sweepResponseSlide(Hit sweepResult, glm::vec2 vEntityVel)
{
    float fDotProd = glm::dot(vEntityVel, sweepResult.vNormal) * sweepResult.fRemainingTime;

    return glm::vec2(sweepResult.vNormal * fDotProd);
}



glm::vec2 Collision::CircleSquare(Circle c, glm::vec2 vVel, const std::vector<NavTile> &aNavTiles, glm::ivec2 vWorldSize)
{
    glm::vec2 vPotentialPos = c.vPos + vVel;

    glm::ivec2 vCurCell = glm::floor(c.vPos);
    glm::ivec2 vTargetCell = vPotentialPos;
    glm::ivec2 vAreaTL = glm::max(glm::min(vCurCell, vTargetCell) - glm::ivec2(1, 1), glm::ivec2(0, 0));
    glm::ivec2 vAreaBR = glm::min(glm::max(vCurCell, vTargetCell) + glm::ivec2(1, 1), vWorldSize);

    glm::ivec2 vCell;
    for (vCell.y = vAreaTL.y; vCell.y <= vAreaBR.y; vCell.y++)
    {
        for (vCell.x = vAreaTL.x; vCell.x <= vAreaBR.x; vCell.x++)
        {
            if (vCell.x >= vWorldSize.x || vCell.y >= vWorldSize.y)
                break;

            int nCurIndex = vCell.y * vWorldSize.x + vCell.x;

            if (aNavTiles.at(nCurIndex).bSolid)
            {
                glm::vec2 vNearestPoint;

                vNearestPoint.x = std::max((float)vCell.x, std::min(vPotentialPos.x, (float)(vCell.x + 1)));
                vNearestPoint.y = std::max((float)vCell.y, std::min(vPotentialPos.y, (float)(vCell.y + 1)));

                glm::vec2 vRayToNearest = vNearestPoint - vPotentialPos;
                float fOverlap = c.fRadius - glm::length(vRayToNearest);

                if (std::isnan(fOverlap))
                {
                    fOverlap = 0.0f;
                }

                if (fOverlap > 0)
                {
                    glm::vec2 vNorm = glm::normalize(vRayToNearest);

                    if (std::isnan(vNorm.x))
                    {
                        vNorm.x = 0.0f;
                    }
                    if (std::isnan(vNorm.y))
                    {
                        vNorm.y = 0.0f;
                    }

                    vPotentialPos = vPotentialPos - vNorm * fOverlap;
                }
            }
        }
    }

    return vPotentialPos;
}
