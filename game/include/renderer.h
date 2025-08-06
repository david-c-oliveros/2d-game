#pragma once

#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include "glad/glad.h"

#include "shader.h"
#include "camera.h"



class Renderer
{
    public:
        static int CreateWindow(std::string sName, sf::Vector2u vDim, sf::State state);

        static void Clear(sf::Color color);
        static void DrawGL(GLuint &vao, const GLShader &cShader, GLuint nNumVert);
        static void Display();

        static sf::RenderWindow& GetWindow();
        static glm::vec2 GetCanvasSize();
        static sf::Vector2i GetCursorScreenPos();
        static sf::Vector2f GetCursorWorldPos(sf::View cameraView);

        static void SetView(sf::View cView);
        static void SetDefaultView();

        static void SetViewMatrix(const std::string sShader);
        static void SetZoom(float fZoom, const std::string sShader);
        static void SetProjectionMatrix(const std::string sShader);

        static glm::mat4 GetProjectionMatrix();
        static glm::mat4 GetViewMatrix();

        static void OnWindowResize(sf::Vector2u vNewSize);

        static glm::mat4 LookAtFromSFView();

        template <typename T>
        static void Draw(T &drawable)
        {
            m_pWindow->pushGLStates();
            m_pWindow->draw(drawable);
            m_pWindow->popGLStates();
        }

    private:
        static std::shared_ptr<sf::RenderWindow> m_pWindow;
        static glm::mat4 m_mViewMatrix;
        static glm::mat4 m_mProjectionMatrix;
};
