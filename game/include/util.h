#pragma once

#include <iostream>
#include <random>
#include <SFML/Window.hpp>
#include <glm/glm.hpp>


namespace util
{
    /***************************************/
    /*        Function Declarations        */
    /***************************************/
    static bool IsAnyKeyPressed();

    template <typename T, typename U>
    static T convert_vector(const U& _v);

    template <typename T>
    static T remap(T x, T fromMin, T fromMax, T toMin, T toMax);

    static void SeedRand();

    template <typename T>
    static T GenRand();

    template <typename T>
    static T GenRandInRange(T range_min, T range_max);

    static uint32_t GetTimeSec();

    template <typename T>
    static void Log(const T &data, bool bNewline = true);

    template <typename T>
    static void LogError(const T &data, bool bNewline = true);




    /**************************************/
    /*        Function Definitions        */
    /**************************************/
    static bool IsAnyKeyPressed()
    {
        for (int k = 0; k < sf::Keyboard::KeyCount; k++)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
            {
                return true;
            }
        }

        return false;
    }



    template <typename T, typename U>
    static T convert_vector(const U& _v)
    {
        return T(_v.x, _v.y);
    }



    template <typename T>
    static T remap(T x, T fromMin, T fromMax, T toMin, T toMax)
    {
        T _x = static_cast<T>(x);
        T _fromMin = static_cast<T>(fromMin);
        T _fromMax = static_cast<T>(fromMax);
        T _toMin = static_cast<T>(toMin);
        T _toMax = static_cast<T>(toMax);

        return (_toMin + (_x - _fromMin) * (_toMax - _toMin) / (_fromMax - _fromMin));
    }



    template <typename T>
    static T GenRandInRange(T range_min, T range_max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(range_min, range_max);

        return distrib(gen);
    }



    static uint32_t GetTimeSec()
    {
        const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::duration dur = now.time_since_epoch();
        std::chrono::seconds sec = std::chrono::duration_cast<std::chrono::seconds>(dur);
        return static_cast<uint32_t>(sec.count());
    }



    template <typename T>
    static void Log(const T &data, bool bNewline)
    {
        std::ostringstream ss;
        ss << data;

        std::string sGreen("\033[32m");
        std::string sBoldGreen("\033[1;32m");
        std::string sReset("\033[0m");

        if (bNewline)
            std::cout << sGreen << "- " << sReset << ss.str() << '\n';
        else
            std::cout << sGreen << "- " << sReset << ss.str();
    }



    template <typename T>
    static void LogError(const T &data, bool bNewline)
    {
        std::ostringstream ss;
        ss << data;

        std::string sRed("\033[31m");
        std::string sBoldRed("\033[1;31m");
        std::string sReset("\033[0m");

        if (bNewline)
            std::cerr << "- " << sBoldRed << "ERROR" << sReset << "::" << ss.str() << '\n';
        else
            std::cerr << "- " << sRed << "ERROR" << sReset << "::" << ss.str();
    }
}
