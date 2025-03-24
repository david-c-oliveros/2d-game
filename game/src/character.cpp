#include "character.h"



Character::Character(glm::vec2 _vWorldPos)
    : Entity(_vWorldPos)
{
}



Character::~Character()
{
}



void Character::Update()
{
    if (bAnimated)
    {
        AnimationManager::update(m_sCurrentAnimation, *m_pSprite);
    }
}



void Character::Draw(sf::RenderWindow &cWindow, TileWorld &cTileWorld)
{
    glm::vec2 _vScalar = cTileWorld.GetWorldScale();
    glm::vec2 _vScreenPos = cTileWorld.WorldToScreen(vWorldPos);

    m_pSprite->setPosition(Util::glm_to_sf_vec2(_vScreenPos));
    m_pSprite->setScale(Util::glm_to_sf_vec2(_vScalar * m_vDefaultScalar));

    cWindow.draw(*m_pSprite);
}



void Character::AttachAnimatedSprite(std::string _sFilepath, glm::ivec2 _vSpriteSize, glm::ivec2 _vFrameRect)
{
    m_vDefaultScalar /= _vSpriteSize;
    bAnimated = true;
    m_pTexture = std::make_unique<sf::Texture>(_sFilepath);

    m_pSprite = std::make_unique<sf::Sprite>(*m_pTexture);

    m_vFrameRect = Util::glm_to_sf_ivec2(_vFrameRect);
    m_vSpriteSize = Util::glm_to_sf_ivec2(_vSpriteSize);
}



void Character::AddAnimation(std::string _sName, glm::ivec2 _vStartIndex, glm::ivec2 _vEndIndex)
{
    AnimationManager::addAnimation(_sName, *m_pTexture, m_vFrameRect, m_vSpriteSize);
    AnimationManager::setAnimationStartingIndex(_sName, Util::glm_to_sf_ivec2(_vStartIndex));
    AnimationManager::setAnimationEndingIndex(_sName, Util::glm_to_sf_ivec2(_vEndIndex));
//    AnimationManager::update(_sName, *m_pSprite);
}



void Character::SetAnimationFrequency(std::string _sName, int32_t _nFreq)
{
    AnimationManager::setAnimationFrequency(_sName, _nFreq);
}



void Character::SetCurrentAnimation(std::string _sName)
{

    m_sPreviousAnimation = m_sCurrentAnimation;
    m_sCurrentAnimation = _sName;

    if (_sName !=  m_sPreviousAnimation)
    {
        AnimationManager::setAnimationIndex(m_sCurrentAnimation,
                                            AnimationManager::getAnimationStartingIndex(m_sCurrentAnimation));
        AnimationManager::forceUpdate(m_sCurrentAnimation, *m_pSprite);
    }
}
