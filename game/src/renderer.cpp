#include <glm/gtx/string_cast.hpp>

#include "renderer.h"
#include "resource_manager.h"
#include "gl_error_manager.h"


std::shared_ptr<sf::RenderWindow> Renderer::m_pWindow;
glm::mat4 Renderer::m_mViewMatrix;



int Renderer::CreateWindow(std::string sName, sf::Vector2u vDim, sf::State state)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antiAliasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

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

//    GLCall(glEnable(GL_BLEND));
//    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glViewport(0, 0, static_cast<GLsizei>(vDim.x), static_cast<GLsizei>(vDim.y)));

    std::cout << "- Set OpenGL viewport\n";

    return 0;
}



void Renderer::Clear(sf::Color color)
{
    GLCall(glClearColor(color.r, color.g, color.b, color.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}



void Renderer::DrawGL(GLuint &vao, const GLShader &cShader, GLuint nNumVert)
{
    cShader.Bind();

    GLCall(glBindVertexArray(vao));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, nNumVert));
    GLCall(glBindVertexArray(0));
}



void Renderer::Display()
{
    m_pWindow->display();
}



sf::RenderWindow& Renderer::GetWindow()
{
    return *m_pWindow;
}



glm::vec2 Renderer::GetCanvasSize()
{
    return Util::convert_vector<glm::vec2>(m_pWindow->getSize());
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



glm::mat4 Renderer::SetViewMatrix(const GLShader &cShader)
{
    cShader.Bind();
//    const float* matArr = cView.getTransform().getMatrix();
//    glm::mat4 view = glm::make_mat4(matArr);

    glm::vec3 vViewPos(0.0f, 0.0f, 1.0f);
    glm::vec3 vOrigin(0.0f);
    glm::mat4 view = glm::lookAt(vViewPos, vOrigin, glm::vec3(0, 1, 0));
    cShader.SetUniform("u_View", view);

    return view;
}



void Renderer::SetProjectionMatrix(const std::string sShader)
{
    glm::vec2 vCanvasSize(GetCanvasSize());
    glm::mat4 projection = glm::ortho<float>(0.0f, vCanvasSize.x, vCanvasSize.y, 0.0f, -1000.0f, 1000.0f);

    RM::GetShader(sShader).Bind();
    RM::GetShader(sShader).SetUniform("u_Projection", projection);
}



void Renderer::OnWindowResize(sf::Vector2u vNewSize)
{
    //Camera::SetViewParams((sf::Vector2f)vNewSize, (sf::Vector2f)vNewSize / 2.0f);
    GLCall(glViewport(0, 0, static_cast<GLsizei>(vNewSize.x), static_cast<GLsizei>(vNewSize.y)));
}
