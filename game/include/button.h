#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <glm/glm.hpp>

#include "renderer.h"
#include "label.h"



class Button
{
    public:
        Button(std::string sLabelText, sf::Font &_cFont, uint32_t _nFontSize);
        ~Button();

        void Draw();
        void SetFontSize(uint32_t size);
        bool Check(sf::Vector2i vCursorPos);
        void Press();
        void Release();
        void PerformAction();


    private:
        bool isHovered(sf::Vector2i vCursorPos);
        void(*pFuncPtr)(std::string);


    private:
        bool m_bPressed = false;
        sf::Vector2i vVec;
        sf::Rect<int32_t> cButtonRect;
        sf::RectangleShape m_cShape;
        std::unique_ptr<Label> m_pLabel;
};
