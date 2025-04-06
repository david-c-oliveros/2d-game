#pragma once

#include <iostream>
#include <map>
#include <cstdint>
#include <string>

#include <SFML/System.hpp>



struct Timer
{
    uint32_t nCurTick;
    uint32_t nTotalTicks;
    bool bRunning;
    bool bOneShot;
};



class TimeManager
{
    public:
        static void Update();
        static float GetFPS();
        static void NewTimer(std::string sName, uint32_t _nTotalTicks);
        static void StartTimer(std::string sName);
        static void ResetTimer(std::string sName);
        static void SetTimerDuration(std::string sName, uint32_t _nTotalTicks);
        static void SetTimerOneShot(std::string sName, bool _bOneShot);
        static void UpdateAllTimers();
        static bool CheckTimer(std::string sName);
        static void TimerTimeout(std::string sName);


    private:
        static std::map<std::string, Timer> m_timers;
        static sf::Clock m_clock;
        static float m_fFrameTime;
};
