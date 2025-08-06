#pragma once

#include <map>
#include <functional>

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Vector2.hpp>

#include "util.h"
#include "callback_utils.h"
#include "scene.h"
#include "button.h"



class UI
{
    public:
        static void RegisterCallbacks();
        static void UpdateButtons(Scene &cScene, sf::Vector2i vCursorPos);
        static void Render(Scene &cScene);
        static void AddLabel(Scene &cScene, std::string sName, std::string sLabelText);
        static void AddButton(Scene &cScene, std::string sName, std::string sLabelText, std::string sCallbackFunc,
                              sf::Rect<int> _cButtonRect = sf::Rect<int>{{ 0, 0 }, { 0, 0 }});
        static void SetDefaultFont(sf::Font &_cFont);
        static void SetDefaultFontSize(uint32_t size);
        static void HandleInput(Scene &cScene, const std::optional<sf::Event> event);

    public:
        static std::map<std::string, std::shared_ptr<IInvoker>> mFunctionRegistry;
//        static std::map<std::string, std::unique_ptr<Label>> mLabels;
//        static std::map<std::string, std::unique_ptr<Button>> mButtons;

        static std::string m_sHoveredButton;
    private:
        static std::shared_ptr<sf::Font> m_pDefaultFont;
        static uint32_t m_nDefaultFontSize;
        static sf::Vector2<int> m_vDefaultButtonSize;
};
