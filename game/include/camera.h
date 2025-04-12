#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include "util.h"


class Camera
{
    public:
        static void SetViewParams(sf::Vector2f _vSize, sf::Vector2f _vCenter);
        static void HandleMouseInput(std::optional<sf::Event> cEvent);
        static void ZoomViewAt(sf::Vector2i vPixel, float fZoom);
        static void SetSize(sf::Vector2f _vZoom);
        static void SetZoom(float fZoom);
        static void EnableFollow();
        static void DisableFollow();
        static void UpdateFollow();
        static void UpdateFollow(sf::Vector2f _vTarget);

        static sf::View GetView();


    public:
        inline static sf::Vector2f vOldPos = sf::Vector2f(0.0f, 0.0f);
        inline static bool bPanning = false;
        inline static bool bFollow = false;


    private:
        inline static sf::View m_cView;
};
