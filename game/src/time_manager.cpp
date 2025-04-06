#include "time_manager.h"



std::map<std::string, Timer> TimeManager::m_timers;



void TimeManager::NewTimer(std::string sName, uint32_t _nTotalTicks)
{
    if (m_timers.count(sName) > 0)
    {
        std::cout << "ERROR::Timer already exists\n";
        return;
    }

    m_timers[sName] = { 0, _nTotalTicks, false, false };
}



void TimeManager::StartTimer(std::string sName)
{
    m_timers.at(sName).bRunning = true;
}



void TimeManager::ResetTimer(std::string sName)
{
    m_timers.at(sName).bRunning = false;
    m_timers.at(sName).nCurTick = 0;
}



void TimeManager::SetTimerDuration(std::string sName, uint32_t _nTotalTicks)
{
    m_timers.at(sName).nTotalTicks = _nTotalTicks;
}



void TimeManager::SetTimerOneShot(std::string sName, bool _bOneShot)
{
    m_timers.at(sName).bOneShot = _bOneShot;
}



void TimeManager::UpdateAllTimers()
{
    static uint32_t count = 0;
    for (auto t : m_timers)
    {
        if (!t.second.bRunning)
            continue;

        m_timers.at(t.first).nCurTick++;
    }
}



bool TimeManager::CheckTimer(std::string sName)
{
    return m_timers.at(sName).nCurTick >= m_timers.at(sName).nTotalTicks;
}



void TimeManager::TimerTimeout(std::string sName)
{
    if (m_timers.at(sName).bOneShot)
    {
        ResetTimer(sName);
        return;
    }

    ResetTimer(sName);
    StartTimer(sName);
}
