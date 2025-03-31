#pragma once

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>



class Label
{
    public:
        Label(std::string _sText, sf::Font &_cFont);
        ~Label();

        void Draw(sf::RenderWindow &cWindow);
        void SetText(std::string _cText);
        void SetFont(sf::Font &_cFont);
        void SetFontSize(uint32_t size);
        void SetPosition(sf::Vector2f _vPos);


    private:
        std::unique_ptr<sf::Text> m_pText;
};
