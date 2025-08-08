#include "ui.h"


//std::map<std::string, std::unique_ptr<Label>> UI::mLabels;
//std::map<std::string, std::unique_ptr<Button>> UI::mButtons;
std::map<std::string, std::shared_ptr<IInvoker>> UI::mFunctionRegistry;
std::shared_ptr<sf::Font> UI::m_pDefaultFont;
uint32_t UI::m_nDefaultFontSize;
std::string UI::m_sHoveredButton = "";
sf::Vector2<int> UI::m_vDefaultButtonSize = sf::Vector2<int>{ 160, 80 };



void UI::RegisterCallbacks()
{
    mFunctionRegistry["ButtonPressed"] = std::make_shared<Invoker<void(std::string)>>(Callbacks::ButtonPressed);
    mFunctionRegistry["RealButton"] = std::make_shared<Invoker<void(std::string)>>(Callbacks::RealButton);
}



void UI::UpdateButtons(Scene &cScene, sf::Vector2i vCursorPos)
{
    bool bButtonHovered = false;
    for (auto &button : cScene.mButtons)
    {
        if (button.second->Check(vCursorPos))
        {
            bButtonHovered = true;
            m_sHoveredButton = button.first;
        }
    }

    if (!bButtonHovered)
    {
        m_sHoveredButton = "";
    }
}



void UI::Render(Scene &cScene)
{
    uint32_t i = 0;
    uint32_t j = 0;
    for (auto &label : cScene.mLabels)
    {
        label.second->Draw();
    }

    for (auto &button : cScene.mButtons)
    {
        button.second->Draw();
    }
}



void UI::AddLabel(Scene &cScene, std::string sName, std::string sLabelText)
{
    cScene.mLabels[sName] = std::make_unique<Label>(sLabelText, *m_pDefaultFont);
    cScene.mLabels[sName]->SetFontSize(m_nDefaultFontSize);
    cScene.mLabels[sName]->SetPosition(sf::Vector2f(0.0f, (cScene.mLabels.size() - 1) * m_nDefaultFontSize * 1.5));
}



void UI::AddButton(Scene &cScene, std::string sName, std::string sLabelText,
                   std::string sCallbackFunc, sf::Rect<int> _cButtonRect)
{
    if (_cButtonRect == sf::Rect<int>{{0,0},{0,0}})
    {
        _cButtonRect = sf::Rect<int>{{ 0, 0 }, m_vDefaultButtonSize};
    }

    cScene.mButtons[sName] = std::make_unique<Button>(sLabelText, *m_pDefaultFont, m_nDefaultFontSize, _cButtonRect);

    if (sCallbackFunc != "")
    {
        cScene.mButtons[sName]->Callback = std::dynamic_pointer_cast<Invoker<void(std::string)>>(
                mFunctionRegistry[sCallbackFunc])->bind(sName);
    }
    else
    {
        cScene.mButtons[sName]->Callback = std::dynamic_pointer_cast<Invoker<void(std::string)>>(
                mFunctionRegistry["ButtonPressed"])->bind(sName);
    }
}



void UI::SetDefaultFont(sf::Font &_cFont)
{
    m_pDefaultFont = std::make_shared<sf::Font>(_cFont);
}



void UI::SetDefaultFontSize(uint32_t size)
{
    m_nDefaultFontSize = size;
}



void UI::HandleInput(Scene &cScene, const std::optional<sf::Event> event)
{
    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left &&
            m_sHoveredButton != "")
        {
            cScene.mButtons[m_sHoveredButton]->OnPress();
        }
    }
    else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        bool bPerformAction = true;
        if (mouseButtonReleased->button == sf::Mouse::Button::Left &&
            m_sHoveredButton != "")
        {
            cScene.mButtons[m_sHoveredButton]->OnRelease(bPerformAction);
        }
    }
}
