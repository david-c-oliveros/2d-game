#include "character.h"



Character::Character(uint32_t _ID, glm::vec2 _vWorldPos)
    : Entity(_ID, _vWorldPos)
{
    cBox = sf::FloatRect({ _vWorldPos.x, _vWorldPos.y }, Globals::TILE_SIZE);
    eState = CharState::IDLE;
}



Character::Character(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos)
    : sName(_sName), Entity(_ID, _vWorldPos)
{
    cBox = sf::FloatRect({ _vWorldPos.x, _vWorldPos.y }, Globals::TILE_SIZE);
    eState = CharState::IDLE;
}



Character::~Character()
{
}



void Character::Update()
{
    if (bAnimated)
    {
        AnimationManager::update(m_sCurrentAnimation, m_pSprite);
    }

    updateSprite();
    setAnimation();
}



void Character::Move(Map &cMap)
{
    m_vGravVel.y += Globals::GRAVITY;
    vWorldPos = Collision::CircleSquare(cCollider, m_vVelocity, m_vGravVel, cMap.aNavTiles, cMap.vMapSize);
    vWorldPos *= Globals::GLM_TILE_SIZE;
    cCollider.vPos = vWorldPos / Globals::GLM_TILE_SIZE;

    updateBoundingBox();
}



void Character::Draw(const std::string sShader)
{
    SpriteRenderer::Draw(*m_pSprite, sShader);
}



void Character::DrawBoundingBox()
{
    sf::RectangleShape bb(cBox.size);
    bb.setPosition(cBox.position);
    bb.setFillColor(sf::Color(0, 100, 0, 255));
    bb.setOrigin({ Globals::TILE_SIZE.x / 2, Globals::TILE_SIZE.y / 2 });

//    Renderer::Draw(bb);
}



void Character::DrawCollider()
{
    sf::CircleShape cColShape(cCollider.fRadius * Globals::TILE_SIZE.x);

    cColShape.setOrigin({ cCollider.fRadius * Globals::TILE_SIZE.x, cCollider.fRadius * Globals::TILE_SIZE.y });
    cColShape.setPosition(Util::convert_vector<sf::Vector2f>(cCollider.vPos * Globals::GLM_TILE_SIZE));
    cColShape.setFillColor(sf::Color(0, 0, 100, 255));

//    Renderer::Draw(cColShape);
}



void Character::AttachAnimatedSprite(const std::string _sSpriteName, const std::string sFilepath,
                                     glm::ivec2 _vSpriteSize, glm::ivec2 _vSheetSize)
{
    m_vDefaultScalar /= _vSpriteSize;
    bAnimated = true;
    std::string sSpriteName(sName + "_" + std::to_string(m_ID) + "_" + _sSpriteName);
    RM::LoadTexture(sFilepath, true, sSpriteName);
    m_pSprite = std::make_unique<GLSprite>();
    
    m_pSprite->SetColor(glm::vec4(1.0f));
    m_pSprite->SetTexture(sSpriteName);
    m_pSprite->SetTextureRect(sf::Rect<int>(sf::Vector2i(Globals::TILE_SIZE), { 0, 0 }));

//    m_pSprite->setOrigin({ _vSpriteSize.x / 2, _vSpriteSize.y / 2 });

    m_vSheetSize  = Util::convert_vector<sf::Vector2i>(_vSheetSize);
    m_vSpriteSize = Util::convert_vector<sf::Vector2i>(_vSpriteSize);
}



void Character::AddAnimation(std::string _sName, glm::ivec2 _vStartIndex, glm::ivec2 _vEndIndex)
{
    std::string sAnimName(sName + "_" + _sName);
    AnimationManager::addAnimation(sAnimName, m_pSprite->GetTexture(), m_vSheetSize, m_vSpriteSize);
    AnimationManager::setAnimationStartingIndex(sAnimName, Util::convert_vector<sf::Vector2i>(_vStartIndex));
    AnimationManager::setAnimationEndingIndex(sAnimName, Util::convert_vector<sf::Vector2i>(_vEndIndex));
}



void Character::SetAnimationFrequency(std::string _sName, int32_t _nFreq)
{
    std::string sAnimName(sName + "_" + _sName);
    AnimationManager::setAnimationFrequency(sAnimName, _nFreq);
}



void Character::SetCurrentAnimation(std::string _sName)
{
    std::string sAnimName(sName + "_" + _sName);
    m_sPreviousAnimation = m_sCurrentAnimation;
    m_sCurrentAnimation = sAnimName;

    if (sAnimName !=  m_sPreviousAnimation)
    {
        AnimationManager::setAnimationIndex(m_sCurrentAnimation,
                                            AnimationManager::getAnimationStartingIndex(m_sCurrentAnimation));
        AnimationManager::forceUpdate(m_sCurrentAnimation, m_pSprite);
    }
}



std::string Character::GetCurrentAnimation()
{
    return m_sCurrentAnimation;
}



sf::Vector2i Character::GetSpriteSize()
{
    return m_vSpriteSize;
}



void Character::setVelocity(glm::vec2 _vVel, float fScalar)
{
    if (_vVel == glm::vec2(0.0f))
    {
        m_vVelocity = glm::vec2(0.0f);
        return;
    }

    m_vVelocity = glm::normalize(_vVel) * fScalar;
}



void Character::updateSprite()
{
    m_pSprite->SetPosition(vWorldPos);
}



void Character::updateBoundingBox()
{
    cBox.position = sf::Vector2(vWorldPos.x, vWorldPos.y);
}



void Character::setAnimation()
{
    /*****************************************/
    /*        Set animation direction        */
    /*****************************************/
    switch(eDir)
    {
        case MoveDir::FORWARD:
            SetCurrentAnimation("walk_forward");
            break;
        case MoveDir::BACK:
            SetCurrentAnimation("walk_back");
            break;
        case MoveDir::LEFT:
            SetCurrentAnimation("walk_left");
            break;
        case MoveDir::RIGHT:
            SetCurrentAnimation("walk_right");
            break;

        case::MoveDir::FORWARD_LEFT:
            SetCurrentAnimation("walk_left");
            break;
        case::MoveDir::FORWARD_RIGHT:
            SetCurrentAnimation("walk_right");
            break;
        case::MoveDir::BACK_LEFT:
            SetCurrentAnimation("walk_left");
            break;
        case::MoveDir::BACK_RIGHT:
            SetCurrentAnimation("walk_right");
            break;
    }
}
