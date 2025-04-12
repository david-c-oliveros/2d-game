#include "renderer.h"
#include "gl_error_manager.h"


std::shared_ptr<sf::RenderWindow> Renderer::m_pWindow;



int Renderer::CreateWindow(std::string sName, sf::Vector2u vDim, sf::State state)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 0;
    settings.majorVersion = 4;
    settings.minorVersion = 6;

    m_pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(vDim), sName, sf::Style::Default, state, settings);
    m_pWindow->setVerticalSyncEnabled(true);
    m_pWindow->setPosition((sf::Vector2i)vDim - (sf::Vector2i)vDim / 2);

    if (!m_pWindow->setActive(true))
    {
        std::cout << "- ERROR::SFML::Failed to set window to active\n";
        return -1;
    }

    gladLoadGL();
    std::cout << "- Activated render window\n";

    GLCall(glViewport(0, 0, static_cast<GLsizei>(vDim.x), static_cast<GLsizei>(vDim.y)));

    std::cout << "- Set OpenGL viewport\n";

    return 0;
}



void Renderer::Clear(sf::Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void Renderer::DrawGL(GLuint &vao, const sf::Shader &shader, GLuint nNumVert)
{
    sf::Shader::bind(&shader);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, nNumVert);
    glBindVertexArray(0);

    sf::Shader::bind(nullptr);
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



void Renderer::OnWindowResize(sf::Vector2u vNewSize)
{
    //Camera::SetViewParams((sf::Vector2f)vNewSize, (sf::Vector2f)vNewSize / 2.0f);
    GLCall(glViewport(0, 0, static_cast<GLsizei>(vNewSize.x), static_cast<GLsizei>(vNewSize.y)));
}
