#include "camera.h"
#include "renderer.h"

#include <glm/gtx/string_cast.hpp>



sf::Vector2f Camera::vOldPos = sf::Vector2f(0.0f, 0.0f);
bool Camera::bPanning = false;
bool Camera::bFollow = false;
uint32_t Camera::nCount = 0;

sf::View Camera::m_cView;
sf::Vector2f Camera::m_vBaseSize;
float Camera::m_fZoom = 1.0f;



void Camera::SetViewParams(sf::Vector2f _vSize, sf::Vector2f _vCenter)
{
    m_vBaseSize = _vSize;
    m_cView.setSize(_vSize);
    m_cView.setCenter(_vCenter);
}



void Camera::HandleMouseInput(std::optional<sf::Event> cEvent)
{
    if (const auto* mouseButtonPressed = cEvent->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
        {
            bPanning  = true;
            vOldPos = Renderer::GetWindow().mapPixelToCoords(sf::Vector2i(mouseButtonPressed->position.x, mouseButtonPressed->position.y));
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

        const sf::Vector2f vNewPos = Renderer::GetWindow().mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
        const sf::Vector2f vDeltaPos = vOldPos - vNewPos;

        m_cView.setCenter(m_cView.getCenter() + vDeltaPos);
        Renderer::SetView(m_cView);

        vOldPos = Renderer::GetWindow().mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));

//        Renderer::SetProjectionMatrix("map_shader");
    }

    else if (const auto* mouseWheelScrolled = cEvent->getIf<sf::Event::MouseWheelScrolled>())
    {
        if (bPanning)
            return;

        const sf::Vector2i _vCursorLocalPosition = sf::Mouse::getPosition(Renderer::GetWindow());

        if (mouseWheelScrolled->delta < 0)
            ZoomViewAt(_vCursorLocalPosition, 2.0f);
        else
            ZoomViewAt(_vCursorLocalPosition, 0.5f);
    }
}


void Camera::ZoomViewAt(sf::Vector2i vPixel, float fZoomFactor)
{
    sf::Vector2f vPixelF = static_cast<sf::Vector2f>(vPixel);

    m_fZoom *= fZoomFactor;
    const sf::Vector2f vBeforeCoord{ Renderer::GetWindow().mapPixelToCoords(vPixel, m_cView) };

    //m_cView.zoom(fZoom);
    m_cView.setSize(m_vBaseSize * m_fZoom);
    Renderer::SetView(m_cView);

    const sf::Vector2f vAfterCoord{ Renderer::GetWindow().mapPixelToCoords(vPixel, m_cView) };
    const sf::Vector2f vOffsetCoords{ vBeforeCoord - vAfterCoord };

    m_cView.move(vOffsetCoords);
    Renderer::SetView(m_cView);

    Renderer::SetProjectionMatrix("map_shader");
}



void Camera::SetSize(sf::Vector2f _vSize)
{
    m_cView.setSize(_vSize);
    m_cView.setCenter(sf::Vector2f(_vSize / 2.0f));
    Renderer::SetView(m_cView);
}



void Camera::SetZoom(float fZoom)
{
    // TODO - Please fix
    m_fZoom = fZoom;
    ZoomViewAt((sf::Vector2i)m_cView.getCenter(), fZoom);
    //m_cView.setCenter((sf::Vector2f)m_cView.getSize() / 2.0f);
    //Renderer::SetView(m_cView);
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
    if (!bFollow || bPanning)
        return;

    sf::Vector2f vNewCenter(Util::convert_vector<sf::Vector2f>(glm::mix(Util::convert_vector<glm::vec2>(m_cView.getCenter()), Util::convert_vector<glm::vec2>(_vTarget), 0.1f)));
    m_cView.setCenter(sf::Vector2f(vNewCenter.x, vNewCenter.y));
}



sf::View Camera::GetView()
{
    return m_cView;
}
