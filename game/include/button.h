#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "util.h"
#include "label.h"



class Button
{
    public:
        Button(std::string sLabelText, sf::Font &_cFont, uint32_t _nFontSize);
        ~Button();

        void Draw(sf::RenderWindow &cWindow);
        void SetFontSize(uint32_t size);
        void Check(sf::Vector2i vCursorPos);


    private:
        bool isHovered(sf::Vector2i vCursorPos);


    private:
        sf::Vector2i vVec;
        sf::Rect<int32_t> cButtonRect;
        sf::RectangleShape m_cShape;
        std::unique_ptr<Label> m_pLabel;
};
