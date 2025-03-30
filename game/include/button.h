#pragma once

#include <SFML/Graphics.hpp>

#include "label.h"



class Button
{
    public:
        Button(std::string sLabelText, sf::Font &_cFont, uint32_t _nFontSize);
        ~Button();

        void Draw(sf::RenderWindow &cWindow);


    private:
        bool isHovered();


    private:
        sf::RectangleShape m_cShape;
        std::unique_ptr<Label> m_pLabel;
};
