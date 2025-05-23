#include "npc.h"
#include "util.h"



Npc::Npc(uint32_t _vID, glm::vec2 _vWorldPos)
    : Character(_vID, _vWorldPos)
{
    sName = std::string("Npc_" + std::to_string(_vID));

    eState = CharState::WALK;
    makeDecision();

    int decision_time = util::GenRandInRange(60, 240);
    TimeManager::NewTimer(sName, decision_time);
    TimeManager::StartTimer(sName);
}



Npc::Npc(uint32_t _vID, std::string _sName, glm::vec2 _vWorldPos)
    : Character(_vID, _sName, _vWorldPos)
{
    sName = std::string(_sName + "_" + std::to_string(_vID));

    eState = CharState::WALK;
    makeDecision();

    int decision_time = util::GenRandInRange(60, 240);
    TimeManager::NewTimer(sName, decision_time);
    TimeManager::StartTimer(sName);
}



Npc::~Npc()
{
}



void Npc::Update(Map &cMap)
{
    if (TimeManager::CheckTimer(sName))
    {
        TimeManager::TimerTimeout(sName);
        makeDecision();
    }

    Character::Update();
    Move(cMap);
}



void Npc::Move(Map &cMap)
{
    /*********************************/
    /*        Update position        */
    /*********************************/
    Character::Move(cMap);

    //m_vVelocity = glm::vec2(0.0f);
}



void Npc::makeDecision()
{
    int decision = util::GenRandInRange(0, 8);
    eDir = static_cast<MoveDir>(decision);
    setVelocity(Globals::aMoveVels[static_cast<size_t>(eDir)], m_fSpeedScalar);
}
