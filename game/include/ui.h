#pragma once

#include <map>

#include <SFML/Graphics.hpp>



class UI
{
    public:
        static void Render(sf::RenderWindow &cWindow);
        static void AddText(std::string sName, std::string sContents);
        static void SetFont(sf::Font &_cFont);
        static void SetFontSize(int size);

    public:
        static std::map<std::string, std::unique_ptr<sf::Text>> mTexts;

    private:
        static sf::Font cFont;
};
