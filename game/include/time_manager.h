#pragma once

#include <iostream>
#include <map>
#include <cstdint>
#include <string>



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
};
