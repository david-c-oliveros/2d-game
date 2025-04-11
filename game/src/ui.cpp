#include "ui.h"


std::map<std::string, std::unique_ptr<Label>> UI::mLabels;
std::map<std::string, std::unique_ptr<Button>> UI::mButtons;
std::shared_ptr<sf::Font> UI::m_pDefaultFont;
uint32_t UI::m_nDefaultFontSize;
std::string UI::m_sHoveredButton = "";



void UI::UpdateButtons(sf::Vector2i vCursorPos)
{
    for (auto &button : mButtons)
    {
        if (button.second->Check(vCursorPos))
        {
            m_sHoveredButton = button.first;
        }
        else
        {
            m_sHoveredButton = "";
        }
    }
}



void UI::Render()
{
    for (auto &label : mLabels)
    {
        label.second->Draw();
    }

    for (auto &button : mButtons)
    {
        button.second->Draw();
    }
}



void UI::AddText(std::string sName, std::string _sLabelText)
{
    mLabels[sName] = std::make_unique<Label>(_sLabelText, *m_pDefaultFont);
    mLabels[sName]->SetFontSize(m_nDefaultFontSize);
    mLabels[sName]->SetPosition(sf::Vector2f(0.0f, (mLabels.size() - 1) * m_nDefaultFontSize * 1.5));
}



//void UI::AddButton(std::string sName, std::string sLabel, sf::Rect cPosDim)
void UI::AddButton(std::string sName)
{
    mButtons[sName] = std::make_unique<Button>("My Button", *m_pDefaultFont, m_nDefaultFontSize);
}



//void UI::SetButtonCallback(std::string sName, (void)(Game::*)(std::string) *pFuncPtr)
//{
//    mButtons[sName].pFuncPtr = _pFuncPtr;
//}



void UI::SetDefaultFont(sf::Font &_cFont)
{
    m_pDefaultFont = std::make_shared<sf::Font>(_cFont);
}



void UI::SetDefaultFontSize(uint32_t size)
{
    m_nDefaultFontSize = size;
}



void UI::HandleInput(sf::RenderWindow &cWindow, const std::optional<sf::Event> event)
{
    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left &&
            m_sHoveredButton != "")
        {
            mButtons[m_sHoveredButton]->Press();
        }
    }
    else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left &&
            m_sHoveredButton != "")
        {
            mButtons[m_sHoveredButton]->Release();
        }
    }
}
