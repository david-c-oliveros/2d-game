#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#include "util.h"

// DEBUG
#include "sprite_renderer.h"
//


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
        static void UpdateFollow(sf::Vector2f _vTarget);

        static sf::View GetView();


    public:
        static sf::Vector2f vOldPos;
        static bool bPanning;
        static bool bFollow;


    private:
        static uint32_t nCount;
        static sf::View m_cView;
        static sf::Vector2f m_vBaseSize;
        static float m_fZoom;
};
