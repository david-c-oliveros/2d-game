#pragma once

#include "util.h"
#include "character.h"



class Npc : public Character
{
    public:
        Npc(uint32_t _vID, glm::vec2 _vWorldPos);
        ~Npc();

        void Update();
        void Animate();
        void Move();


    private:
        void makeDecision();


    private:
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        float m_fSpeedScalar = 1.5f;
        uint8_t nFrameCount = 1;
};
