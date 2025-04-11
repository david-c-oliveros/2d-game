#include "camera.h"



void Camera::SetCameraView(sf::Vector2f _vSize, sf::Vector2f _vCenter)
{
    m_cView.setSize(_vSize);
    m_cView.setCenter(_vCenter);
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

        m_cView.setCenter(m_cView.getCenter() + vDeltaPos);
        cWindow.setView(m_cView);

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

    m_cView.zoom(fZoom);
    cWindow.setView(m_cView);

    const sf::Vector2f vAfterCoord{ cWindow.mapPixelToCoords(vPixel) };
    const sf::Vector2f vOffsetCoords{ vBeforeCoord - vAfterCoord };

    m_cView.move(vOffsetCoords);
    cWindow.setView(m_cView);
}



void Camera::SetSize(sf::RenderWindow &cWindow, sf::Vector2f _vSize)
{
    m_cView.setSize(_vSize);
    m_cView.setCenter(sf::Vector2f(_vSize.x / 2.0f, _vSize.y / 2.0f));
    cWindow.setView(m_cView);
}



void Camera::SetZoom(sf::RenderWindow &cWindow, float _fZoom)
{
    m_cView.zoom(_fZoom);
    m_cView.setCenter(sf::Vector2f(m_cView.getSize().x / 2.0f, m_cView.getSize().y / 2.0f));
    cWindow.setView(m_cView);
}



void Camera::EnableFollow()
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

    sf::Vector2f vNewCenter(Util::convert_vector<sf::Vector2f>(glm::mix(Util::convert_vector<glm::vec2>(m_cView.getCenter()), Util::convert_vector<glm::vec2>(_vTarget), 0.1f)));
    m_cView.setCenter(sf::Vector2f(vNewCenter.x, vNewCenter.y));
}



sf::View Camera::GetView()
{
    return m_cView;
}
