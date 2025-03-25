#include "camera.h"



void Camera::SetCameraView(sf::Vector2f _vSize, sf::Vector2f _vCenter)
{
    cView.setSize(_vSize);
    cView.setCenter(_vCenter);
}



void Camera::HandleMouseInput(sf::RenderWindow &cWindow, std::optional<sf::Event> cEvent)
{
    if (const auto* mouseButtonPressed = cEvent->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
        {
            bPanning  = true;
            vOldPos = cWindow.mapPixelToCoords(sf::Vector2i(mouseButtonPressed->position.x, mouseButtonPressed->position.y));
        }
    }
    else if (const auto* mouseButtonReleased = cEvent->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Middle)
        {
            bPanning = false;
        }
    }

    else if (const auto* mouseMoved = cEvent->getIf<sf::Event::MouseMoved>())
    {
        if (!bPanning)
            return;

        const sf::Vector2f vNewPos = cWindow.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
        const sf::Vector2f vDeltaPos = vOldPos - vNewPos;

        cView.setCenter(cView.getCenter() + vDeltaPos);
        cWindow.setView(cView);

        vOldPos = cWindow.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
    }

    else if (const auto* mouseWheelScrolled = cEvent->getIf<sf::Event::MouseWheelScrolled>())
    {
        if (bPanning)
            return;

        const sf::Vector2i _vCursorLocalPosition = sf::Mouse::getPosition(cWindow);

        if (mouseWheelScrolled->delta < 0)
            ZoomViewAt(cWindow, _vCursorLocalPosition, 2.0f);
        else
            ZoomViewAt(cWindow, _vCursorLocalPosition, 0.5f);
    }
}


void Camera::ZoomViewAt(sf::RenderWindow &cWindow, sf::Vector2i vPixel, float fZoom)
{
    const sf::Vector2f vBeforeCoord{ cWindow.mapPixelToCoords(vPixel) };

    cView.zoom(fZoom);
    cWindow.setView(cView);

    const sf::Vector2f vAfterCoord{ cWindow.mapPixelToCoords(vPixel) };
    const sf::Vector2f vOffsetCoords{ vBeforeCoord - vAfterCoord };

    cView.move(vOffsetCoords);
    cWindow.setView(cView);
}



void Camera::SetSize(sf::RenderWindow &cWindow, sf::Vector2f _vSize)
{
    cView.setSize(_vSize);
    cView.setCenter(sf::Vector2f(_vSize.x / 2.0f, _vSize.y / 2.0f));
    cWindow.setView(cView);
}



void Camera::SetZoom(sf::RenderWindow &cWindow, float _fZoom)
{
    cView.zoom(_fZoom);
    cView.setCenter(sf::Vector2f(cView.getSize().x / 2.0f, cView.getSize().y / 2.0f));
    cWindow.setView(cView);
}



void Game::EnableFollow()
{
    bFollow = true;
}



void Camera::DisableFollow()
{
    bFollow = false;
}



void Camera::UpdateFollow(sf::Vector2f _vTarget)
{
    if (!bFollow)
        return;

    cView.setCenter();
}



void Game::LerpCamera(glm::vec2 _vTarget)
{
    sf::Vector2u _vScreenSize = cWindow.getSize();
    glm::vec2 vCenter = cTileWorld.ScreenToWorld(glm::vec2(_vScreenSize.x, _vScreenSize.y));
    glm::vec2 vDelta = glm::mix(vCenter, _vTarget, 0.5f);
}
