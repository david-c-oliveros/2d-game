#pragma once

#include <functional>

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
        Button(std::string sLabelText, sf::Font &_cFont, uint32_t _nFontSize, sf::Rect<int> _cButtonRect);
        ~Button();

        void Draw();
        void SetFontSize(uint32_t size);
        bool Check(sf::Vector2i vCursorPos);
        void OnPress();
        void OnRelease(bool bPerformAction);
        std::function<void()> Callback;


    private:
        bool isHovered(sf::Vector2i vCursorPos);
        void(*pFuncPtr)(std::string);


    public:
        bool bPressed = false;

    private:
        sf::Vector2i vVec;
        sf::Rect<int32_t> cButtonRect;
        sf::RectangleShape m_cShape;
        std::unique_ptr<Label> m_pLabel;
};
