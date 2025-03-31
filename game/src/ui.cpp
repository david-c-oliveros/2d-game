#include "ui.h"


std::map<std::string, std::unique_ptr<Label>> UI::mLabels;
std::map<std::string, std::unique_ptr<Button>> UI::mButtons;
std::shared_ptr<sf::Font> UI::m_pDefaultFont;
uint32_t UI::m_nDefaultFontSize;



void UI::UpdateButtons(sf::Vector2i vCursorPos)
{
    for (auto &button : mButtons)
    {
        button.second->Check(vCursorPos);
    }
}



void UI::Render(sf::RenderWindow &cWindow)
{
    for (auto &label : mLabels)
    {
        label.second->Draw(cWindow);
    }

    for (auto &button : mButtons)
    {
        button.second->Draw(cWindow);
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



void UI::SetDefaultFont(sf::Font &_cFont)
{
    m_pDefaultFont = std::make_shared<sf::Font>(_cFont);
}



void UI::SetDefaultFontSize(uint32_t size)
{
    m_nDefaultFontSize = size;
}
