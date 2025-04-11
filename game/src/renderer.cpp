#include "renderer.h"



std::shared_ptr<sf::RenderWindow> Renderer::m_pWindow;



int Renderer::CreateWindow(std::string sName, sf::Vector2u vDim, sf::State state)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 4;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    m_pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(vDim), sName, sf::Style::Default, state, settings);
    m_pWindow->setVerticalSyncEnabled(true);
    m_pWindow->setPosition((sf::Vector2i)vDim - (sf::Vector2i)vDim / 2);

    if (!m_pWindow->setActive(true))
    {
        std::cout << "Failed to set window to active\n";
        return -1;
    }

    //glViewport(0, 0, static_cast<GLsizei>(m_pWindow->getSize().x), static_cast<GLsizei>(m_pWindow->getSize().y));
}



void Renderer::Clear(sf::Color color)
{
//    glClearColor(color.r, color.g, color.b, color.a);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_pWindow->clear(color);
}



void Renderer::Display()
{
    m_pWindow->display();
}



sf::RenderWindow& Renderer::GetWindow()
{
    return *m_pWindow;
}



sf::Vector2i Renderer::GetCursorScreenPos()
{
    return sf::Vector2i(sf::Mouse::getPosition(*m_pWindow));
}



sf::Vector2f Renderer::GetCursorWorldPos(sf::View cameraView)
{
    m_pWindow->setView(cameraView);
    return m_pWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_pWindow));
}



void Renderer::SetView(sf::View cView)
{
    m_pWindow->setView(cView);
}




void Renderer::SetDefaultView()
{
    m_pWindow->setView(m_pWindow->getDefaultView());
}
