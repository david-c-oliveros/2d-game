#include "button.h"
#include "util.h"



Button::Button(std::string _sLabelText, sf::Font &_cFont, uint32_t _nFontSize, sf::Rect<int> _cButtonRect)
{
//    cButtonRect = sf::Rect<int32_t>({ 40, 400}, { 160, 80 });

    cButtonRect = _cButtonRect;

    m_cShape = sf::RectangleShape(util::convert_vector<sf::Vector2f>(cButtonRect.size));
    m_cShape.setFillColor(sf::Color::Cyan);
    m_cShape.setPosition(util::convert_vector<sf::Vector2f>(cButtonRect.position));

    m_pLabel = std::make_unique<Label>(_sLabelText, _cFont);
    m_pLabel->SetFontSize(_nFontSize);
    m_pLabel->SetPosition(util::convert_vector<sf::Vector2f>(cButtonRect.position));
}


Button::~Button()
{
}


void Button::Draw()
{
    Renderer::Draw(m_cShape);
    m_pLabel->Draw();
}



void Button::SetFontSize(uint32_t size)
{
    m_pLabel->SetFontSize(size);
}



bool Button::Check(sf::Vector2i vCursorPos)
{
    if (isHovered(vCursorPos) && !bPressed)
    {
        m_cShape.setFillColor(sf::Color::Blue);
        return true;
    }
    else if (isHovered(vCursorPos) && bPressed)
    {
        return true;
    }

    bPressed = false;
    m_cShape.setFillColor(sf::Color::Cyan);
    return false;
}



void Button::OnPress()
{
    m_cShape.setFillColor(sf::Color::Green);
    bPressed = true;
}



void Button::OnRelease(bool bPerformAction)
{
    util::Log("Button released");
    m_cShape.setFillColor(sf::Color::Cyan);

    if (bPressed)
    {
        Callback();
    }
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
