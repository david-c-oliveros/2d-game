#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
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
        static glm::mat4 SetViewMatrix(const GLShader &cShader);
        static void SetProjectionMatrix(const std::string sShader);

        static void OnWindowResize(sf::Vector2u vNewSize);

        template <typename T>
        static void Draw(T &drawable, sf::Shader &cShader)
        {
            m_pWindow->pushGLStates();
            m_pWindow->draw(drawable, &cShader);
            m_pWindow->popGLStates();
        }

        static glm::mat4 m_mViewMatrix;

    private:
        static std::shared_ptr<sf::RenderWindow> m_pWindow;
};
