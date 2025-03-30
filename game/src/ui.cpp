#include "ui.h"


std::map<std::string, std::unique_ptr<sf::Text>> UI::mTexts;
sf::Font UI::cFont;



void UI::Render(sf::RenderWindow &cWindow)
{
    for (auto &text : mTexts)
    {
        cWindow.draw(*text.second);
    }
}



void UI::AddText(std::string sName, std::string sContents)
{
    mTexts[sName] = std::make_unique<sf::Text>(cFont, sContents);
    mTexts[sName]->setPosition(sf::Vector2f(0.0f, (mTexts.size() - 1) * 100.0f));
}



void UI::SetFont(sf::Font &_cFont)
{
    cFont = _cFont;
}



void UI::SetFontSize(int size)
{
    for (auto &text : mTexts)
    {
        text.second->setCharacterSize(size);
    }
}
