#pragma once

#include <SFML/Graphics.hpp>


class Camera
{
    public:
        static void SetCameraView(sf::Vector2f _vSize, sf::Vector2f _vCenter);
        static void HandleMouseInput(sf::RenderWindow &cWindow, std::optional<sf::Event> cEvent);
        static void ZoomViewAt(sf::RenderWindow &cWindow, sf::Vector2i vPixel, float fZoom);
        static void SetSize(sf::RenderWindow &cWindow, sf::Vector2f _vZoom);
        static void SetZoom(sf::RenderWindow &cWindow, float _fZoom);
        static void EnableFollowCamera();
        static void DisableFollowCamera();
        static void UpdateFollowCamera();


    public:
        inline static sf::View cView;
        inline static sf::Vector2f vOldPos = sf::Vector2f(0.0f, 0.0f);
        inline static bool bPanning = false;
        inline static bool bFollow = false;
};
