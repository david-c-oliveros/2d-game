#include "label.h"

#include <glm/gtx/string_cast.hpp>
#include "util.h"



Label::Label(std::string _sText, sf::Font &_cFont)
{
    m_pText = std::make_unique<sf::Text>(_cFont, _sText);
}


Label::~Label()
{
}


void Label::Draw()
{
    Renderer::Draw(*m_pText);
}


void Label::SetText(std::string _sText)
{
    m_pText->setString(_sText);
}


void Label::SetFont(sf::Font &_cFont)
{
    m_pText->setFont(_cFont);
}


void Label::SetFontSize(uint32_t size)
{
    m_pText->setCharacterSize(size);
}


void Label::SetOrigin(sf::Vector2f vOrigin)
{
    m_pText->setOrigin(vOrigin);
}


void Label::SetPosition(sf::Vector2f _vPos)
{
    m_pText->setPosition(_vPos);
}


sf::Text Label::GetSFText()
{
    return *m_pText;
}
