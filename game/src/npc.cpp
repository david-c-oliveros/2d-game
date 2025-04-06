#include "npc.h"



Npc::Npc(uint32_t _vID, glm::vec2 _vWorldPos)
    : Character(_vID, _vWorldPos)
{
    eState = CharState::WALK;
    makeDecision();

    sName = std::string("Npc_" + std::to_string(_vID));

    TimeManager::NewTimer(sName, 120);
    TimeManager::StartTimer(sName);
}



Npc::Npc(uint32_t _vID, std::string _sName, glm::vec2 _vWorldPos)
    : Character(_vID, _sName, _vWorldPos)
{
    eState = CharState::WALK;
    makeDecision();

    sName = std::string(_sName + "_" + std::to_string(_vID));

    TimeManager::NewTimer(sName, 120);
    TimeManager::StartTimer(sName);
}



Npc::~Npc()
{
}



void Npc::Update()
{
    Character::Update();
    setAnimation();
    Move();

    if (TimeManager::CheckTimer(sName))
    {
        TimeManager::TimerTimeout(sName);
        makeDecision();
    }
}



void Npc::Move()
{
    /*********************************/
    /*        Update position        */
    /*********************************/
    if (glm::length(m_vVelocity) > 0.0f)
    {
        vWorldPos += glm::normalize(m_vVelocity) * m_fSpeedScalar;
    }

    //m_vVelocity = glm::vec2(0.0f);
}



void Npc::makeDecision()
{
    std::cout << "Making decision\n";
    int decision = Util::GenRandInRange(0, 8);
    eDir = static_cast<MoveDir>(decision);
    m_vVelocity = aMoveVels[static_cast<size_t>(eDir)];
}
