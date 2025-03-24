#pragma once

#include <glm/gtx/string_cast.hpp>

#include "character.h"



class Player : public Character
{
    public:
        Player(glm::vec2 _vWorldPos);
        ~Player();

        void Update();


    private:
        void handleInput();


    private:
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        float m_speedScalar = 0.16f;
        uint8_t nFrameCount = 0;
};
