#pragma once

#include <map>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "button.h"



class UI
{
    public:
        static void UpdateButtons(sf::Vector2i vCursorPos);
        static void Render(sf::RenderWindow &cWindow);
        static void AddText(std::string sName, std::string sContents);
//        static void AddButton(std::string sName, std::string sLabel, sf::Rect cPosDim);
        static void AddButton(std::string sName);
        static void SetDefaultFont(sf::Font &_cFont);
        static void SetDefaultFontSize(uint32_t size);
        static void HandleInput(sf::RenderWindow &cWindow, const std::optional<sf::Event> event);
//        static void SetButtonCallback(std::string sName, void(Game::*)(std::string) *pFuncPtr);

    public:
        static std::map<std::string, std::unique_ptr<Label>> mLabels;
        static std::map<std::string, std::unique_ptr<Button>> mButtons;

    private:
        static std::shared_ptr<sf::Font> m_pDefaultFont;
        static uint32_t m_nDefaultFontSize;
        static std::string m_sHoveredButton;
};
