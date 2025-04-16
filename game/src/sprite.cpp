#include "sprite.h"

#include <glm/gtx/string_cast.hpp>
#include "util.h"


GLSprite::GLSprite()
    : m_sTexName("")
{
}


GLSprite::GLSprite(const std::string sTexName)
    : m_sTexName(sTexName)
{
}


GLSprite::~GLSprite()
{
}


void GLSprite::SetTexture(const std::string sTexName)
{
    m_sTexName = sTexName;
}


void GLSprite::SetColor(glm::vec4 vColor)
{
    m_vColor = vColor;
}


void GLSprite::SetTextureRect(sf::Rect<int> _TextureRect)
{
    if (m_sTexName == "")
    {
        util::Log("ERROR::Sprite texture must be set before the texture rectangle");
        return;
    }

    m_TextureRect = _TextureRect;
}


void GLSprite::SetPosition(glm::vec2 vPos)
{
    m_vPos = vPos;
}


const sf::Rect<float> GLSprite::GetTexRectNorm() const
{
    sf::Vector2f vTexSize = util::convert_vector<sf::Vector2f>(RM::GetTexture(m_sTexName).GetSize());

    sf::FloatRect TextureRectNorm((sf::Vector2f)m_TextureRect.position, (sf::Vector2f)m_TextureRect.size);

    TextureRectNorm.position.x /= vTexSize.x;
    TextureRectNorm.position.y /= vTexSize.y;
    TextureRectNorm.size.x     /= vTexSize.x;
    TextureRectNorm.size.y     /= vTexSize.y;

    return TextureRectNorm;
}


const glm::vec2& GLSprite::GetPosition() const
{
    return m_vPos;
}


const GLTexture GLSprite::GetTexture() const
{
    return RM::GetTexture(m_sTexName);
}


const glm::vec4& GLSprite::GetColor() const
{
    return m_vColor;
}


const std::string GLSprite::GetTexName() const
{
    return m_sTexName;
}
