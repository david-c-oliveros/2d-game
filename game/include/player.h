#pragma once

#include <glm/gtx/string_cast.hpp>

#include "character.h"



class Player : public Character
{
    public:
        Player(uint32_t _ID, glm::vec2 _vWorldPos);
        Player(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos);
        ~Player();

        void Update();
        void Animate();
        void Move();


    private:
        void handleInput();


    private:
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        float m_fSpeedScalar = 1.5f;
        uint8_t nFrameCount = 1;
};
