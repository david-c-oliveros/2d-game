#include "renderer.h"
#include "resource_manager.h"
#include "gl_error_manager.h"
#include "util.h"


std::shared_ptr<sf::RenderWindow> Renderer::m_pWindow;
glm::mat4 Renderer::m_mViewMatrix;
glm::mat4 Renderer::m_mProjectionMatrix;



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
        util::Log("ERROR::SFML::Failed to set window to active");
        return -1;
    }

    gladLoadGL();
    util::Log("Activated render window");

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glViewport(0, 0, static_cast<GLsizei>(vDim.x), static_cast<GLsizei>(vDim.y)));

    util::Log("Set OpenGL viewport");

    return 0;
}



void Renderer::Clear(sf::Color color)
{
    glm::vec4 c = glm::vec4(color.r, color.g, color.b, color.a);
    c = util::remap(c, glm::vec4(0.0f), glm::vec4(255.0f), glm::vec4(0.0f), glm::vec4(1.0f));

    GLCall(glClearColor(c.r, c.g, c.b, c.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}



void Renderer::DrawGL(GLuint &vao, const GLShader &cShader, GLuint nNumVert)
{
    cShader.Bind();

    GLCall(glBindVertexArray(vao));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, nNumVert));
    GLCall(glBindVertexArray(0));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
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
    return util::convert_vector<glm::vec2>(m_pWindow->getSize());
}



sf::Vector2i Renderer::GetCursorScreenPos()
{
    return sf::Vector2i(sf::Mouse::getPosition(*m_pWindow));
}



sf::Vector2f Renderer::GetCursorWorldPos(sf::View cameraView)
{
    auto oldView = m_pWindow->getView();
    m_pWindow->setView(cameraView);
    auto vWorldPos = m_pWindow->mapPixelToCoords(sf::Mouse::getPosition(*m_pWindow));
    m_pWindow->setView(oldView);

    return vWorldPos;
}



void Renderer::SetView(sf::View cView)
{
    m_pWindow->setView(cView);
}



void Renderer::SetDefaultView()
{
    m_pWindow->setView(m_pWindow->getDefaultView());
}



void Renderer::SetZoom(float fZoom, const std::string sShader)
{
    m_mViewMatrix = glm::scale(m_mViewMatrix, glm::vec3(1.0f / fZoom, 1.0f / fZoom, 1.0f));
    RM::GetShader(sShader).Bind();
    RM::GetShader(sShader).SetUniform("u_View", m_mViewMatrix);
}



void Renderer::SetViewMatrix(const std::string sShader)
{
    glm::mat4 view = LookAtFromSFView();

    RM::GetShader(sShader).Bind();
    RM::GetShader(sShader).SetUniform("u_View", view);

    m_mViewMatrix = view;
}



void Renderer::SetProjectionMatrix(const std::string sShader)
{
    sf::Vector2f vCenter = Camera::GetView().getCenter();
    sf::Vector2f vSize   = Camera::GetView().getSize();

    float fLeft   = -vSize.x / 2;
    float fRight  = vSize.x / 2;
    float fTop    = -vSize.y / 2;
    float fBottom = vSize.y / 2;

    glm::mat4 projection = glm::ortho<float>(fLeft, fRight, fBottom, fTop, -1000.0f, 1000.0f);

    m_mProjectionMatrix = projection;

    RM::GetShader(sShader).Bind();
    RM::GetShader(sShader).SetUniform("u_Projection", projection);
}



glm::mat4 Renderer::GetProjectionMatrix()
{
    return m_mProjectionMatrix;
}



glm::mat4 Renderer::GetViewMatrix()
{
    return m_mViewMatrix;
}



void Renderer::OnWindowResize(sf::Vector2u vNewSize)
{
    //Camera::SetViewParams((sf::Vector2f)vNewSize, (sf::Vector2f)vNewSize / 2.0f);
    GLCall(glViewport(0, 0, static_cast<GLsizei>(vNewSize.x), static_cast<GLsizei>(vNewSize.y)));
}



glm::mat4 Renderer::LookAtFromSFView()
{
    sf::Vector2f vViewCenter = Camera::GetView().getCenter();

    float fRotRad = -Camera::GetView().getRotation().asRadians();

    glm::vec3 vEye(vViewCenter.x, vViewCenter.y, 1.0f);
    glm::vec3 vTarget(vViewCenter.x, vViewCenter.y, 0.0f);
    glm::vec3 vUp(std::sin(fRotRad), std::cos(fRotRad), 0.0f);

    glm::mat4 vLookAt = glm::lookAt(vEye, vTarget, vUp);

    return vLookAt;
}
