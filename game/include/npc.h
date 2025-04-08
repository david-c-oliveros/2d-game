#pragma once

#include <glm/gtx/string_cast.hpp>

#include "globals.h"
#include "util.h"
#include "time_manager.h"
#include "character.h"



class Npc : public Character
{
    public:
        Npc(uint32_t _vID, glm::vec2 _vWorldPos);
        Npc(uint32_t _vID, std::string _sName, glm::vec2 _vWorldPos);
        ~Npc();

        void Update();
        void Move();


    private:
        void makeDecision();


    private:
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        float m_fSpeedScalar = 1.0f;
        uint8_t nFrameCount = 1;
};
