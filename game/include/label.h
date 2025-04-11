#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "renderer.h"



class Label
{
    public:
        Label(std::string _sText, sf::Font &_cFont);
        ~Label();

        void Draw();
        void SetText(std::string _cText);
        void SetFont(sf::Font &_cFont);
        void SetFontSize(uint32_t size);
        void SetPosition(sf::Vector2f _vPos);


    private:
        std::unique_ptr<sf::Text> m_pText;
};
