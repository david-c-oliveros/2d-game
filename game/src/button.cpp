#include "button.h"



Button::Button(std::string _sLabelText, sf::Font &_cFont, uint32_t _nFontSize)
{
    sf::Vector2f vPos({ 40.0f, 400.0f });
    cButtonRect = sf::Rect<int32_t>({ 40, 400}, { 160, 80 });
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
    cWindow.draw(m_cShape);
    m_pLabel->Draw(cWindow);
}



void Button::SetFontSize(uint32_t size)
{
    m_pLabel->SetFontSize(size);
}



bool Button::Check(sf::Vector2i vCursorPos)
{
    if (isHovered(vCursorPos) && !m_bPressed)
    {
        m_cShape.setFillColor(sf::Color::Blue);
        return true;
    }
    else if (isHovered(vCursorPos) && m_bPressed)
    {
        return true;
    }

    m_cShape.setFillColor(sf::Color::Cyan);
    return false;
}



void Button::Press()
{
    pFuncPtr("This button");
    PerformAction();
    m_cShape.setFillColor(sf::Color::Green);
    m_bPressed = true;
}



void Button::Release()
{
    m_cShape.setFillColor(sf::Color::Cyan);
    m_bPressed = false;
}



void Button::PerformAction()
{
    std::cout << "Doing button action\n";
}



bool Button::isHovered(sf::Vector2i vCursorPos)
{
    sf::Vector2i vTL({ cButtonRect.position.x, cButtonRect.position.y });
    sf::Vector2i vBR({ cButtonRect.position.x + cButtonRect.size.x,
                       cButtonRect.position.y + cButtonRect.size.y });

    return vCursorPos.x > vTL.x &&
           vCursorPos.y > vTL.y &&
           vCursorPos.x <= vBR.x &&
           vCursorPos.y <= vBR.y;
}
