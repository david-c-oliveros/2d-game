#include "button.h"



Button::Button(std::string _sLabelText, sf::Font &_cFont, uint32_t _nFontSize)
{
    sf::Vector2f vPos({ 40.0f, 400.0f });
    m_cShape = sf::RectangleShape({ 160.0f, 80.0f });
    m_cShape.setPosition(vPos);

    m_pLabel = std::make_unique<Label>(_sLabelText, _cFont);
    m_pLabel->SetFontSize(_nFontSize);
    m_pLabel->SetPosition(vPos);
}


Button::~Button()
{
}


void Button::Draw(sf::RenderWindow &cWindow)
{
    m_pLabel->Draw(cWindow);
    cWindow.draw(m_cShape);
}
