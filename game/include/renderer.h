#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include "glad/glad.h"

#include "camera.h"



class Renderer
{
    public:
        static int CreateWindow(std::string sName, sf::Vector2u vDim, sf::State state);

        static void Clear(sf::Color color);
        static void DrawGL(GLuint &vao, const sf::Shader &shader, GLuint nNumVert);
        static void Display();

        static sf::RenderWindow& GetWindow();
        static sf::Vector2i GetCursorScreenPos();
        static sf::Vector2f GetCursorWorldPos(sf::View cameraView);

        static void SetView(sf::View cView);
        static void SetDefaultView();

        static void OnWindowResize(sf::Vector2u vNewSize);

        template <typename T>
        static void Draw(T &drawable)
        {
            m_pWindow->draw(drawable);
        }


    private:
        static std::shared_ptr<sf::RenderWindow> m_pWindow;
};
