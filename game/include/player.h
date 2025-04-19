#pragma once

#include "globals.h"
#include "character.h"


class Map;



class Player : public Character
{
    public:
        Player();
        Player(uint32_t _ID, glm::vec2 _vWorldPos);
        Player(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos);
        ~Player();

        void Update(Map &cMap);
        void Move(Map &cMap);


    private:
        void handleInput();
        void updateWorldGridPosition();
        void resolveCollisions(Map &cMap);


    public:
        glm::ivec2 vWorldGridPos;


    private:
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        float m_fSpeedScalar = 2.0f;
        uint8_t nFrameCount = 1;
};
