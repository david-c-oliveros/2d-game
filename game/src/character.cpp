#include "character.h"



Character::Character(uint32_t _ID, glm::vec2 _vWorldPos)
    : Entity(_ID, _vWorldPos)
{
}



Character::Character(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos)
    : sName(_sName), Entity(_ID, _vWorldPos)
{
    std::cout << sName << " ID: " << m_ID << "\n";
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



void Character::Draw(sf::RenderWindow &cWindow)
{
    m_pSprite->setPosition(Util::convert_vector<sf::Vector2f>(vWorldPos));

    cWindow.draw(*m_pSprite);
}



void Character::AttachAnimatedSprite(const std::string _sFilepath, glm::ivec2 _vSpriteSize, glm::ivec2 _vFrameRect)
{
    m_vDefaultScalar /= _vSpriteSize;
    bAnimated = true;
    m_pTexture = std::make_unique<sf::Texture>(_sFilepath);

    m_pSprite = std::make_unique<sf::Sprite>(*m_pTexture);

    m_vFrameRect  = Util::convert_vector<sf::Vector2i>(_vFrameRect);
    m_vSpriteSize = Util::convert_vector<sf::Vector2i>(_vSpriteSize);
}



void Character::AddAnimation(std::string _sName, glm::ivec2 _vStartIndex, glm::ivec2 _vEndIndex)
{
    AnimationManager::addAnimation(_sName, m_pSprite->getTexture(), m_vFrameRect, m_vSpriteSize);
    AnimationManager::setAnimationStartingIndex(_sName, Util::convert_vector<sf::Vector2i>(_vStartIndex));
    AnimationManager::setAnimationEndingIndex(_sName, Util::convert_vector<sf::Vector2i>(_vEndIndex));
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



sf::Vector2i Character::GetSpriteSize()
{
    return m_vSpriteSize;
}
