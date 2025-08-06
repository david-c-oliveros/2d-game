#include "building.h"

#include <SFML/Graphics/Rect.hpp>

// Debug
#include <glm/gtx/string_cast.hpp>

#include "util.h"
#include "sprite.h"
#include "resource_manager.h"
#include "sprite_renderer.h"



Building::Building(uint32_t _ID, const glm::vec2 &_vWorldPos)
    : Entity(_ID, _vWorldPos)
{
}



Building::Building(uint32_t _ID, std::string _sName, const glm::vec2 &_vWorldPos)
    : sName(_sName), Entity(_ID, _vWorldPos)
{
}



Building::Building(Building&& other)
{
    sName = other.sName;
    m_ID = other.m_ID;
    vWorldPos = other.vWorldPos;
    m_pSprite = std::move(other.m_pSprite);
    m_vScale = other.m_vScale;
    m_vSpriteSize = other.m_vSpriteSize;
}




Building::~Building()
{
}



void Building::Update(Map &cMap)
{
}




void Building::Draw(const std::string sShader)
{
    SpriteRenderer::Draw(*m_pSprite, sShader);
}



void Building::DrawDebug(const std::string sShader)
{
    SpriteRenderer::Draw(vWorldPos, sShader);
}



void Building::AttachSprite(const std::string _sSpriteName, const std::string sFilepath,
                            glm::ivec2 _vSpritePos, glm::ivec2 _vSpriteSize)
{
    std::string sSpriteName(sName + "_" + std::to_string(m_ID) + "_" + _sSpriteName);
    RM::LoadTexture(sFilepath, true, sSpriteName);
    m_pSprite = std::make_unique<GLSprite>();

    m_pSprite->SetScale(m_vScale);

    m_vSpriteSize = util::convert_vector<sf::Vector2i>(_vSpriteSize);

    m_pSprite->SetColor(glm::vec4(1.0f));
    m_pSprite->SetTexture(sSpriteName);

    //sf::Rect<int> rect(util::convert_vector<sf::Vector2i>(_vSpritePos), m_vSpriteSize);
    sf::Rect<int> rect({ 0, 0 }, { 32, 32 });

    m_pSprite->SetTextureRect(rect);
    vWorldPos = glm::vec2(0.0f, 0.0f);
    m_pSprite->SetPosition(vWorldPos);
}
