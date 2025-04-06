#include "player.h"



Player::Player(uint32_t _ID, glm::vec2 _vWorldPos)
    : Character(_ID, _vWorldPos)
{
    eDir = MoveDir::BACK;
}



Player::Player(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos)
    : Character(_ID, _sName, _vWorldPos)
{
    eDir = MoveDir::BACK;
}



Player::~Player()
{
}



void Player::Update()
{
    Character::Update();
    handleInput();

    setAnimation();
    Move();
}



void Player::Move()
{
    /*********************************/
    /*        Update position        */
    /*********************************/
    if (eState == CharState::IDLE)
    {
        return;
    }

    assert(static_cast<int>(eDir) < aMoveVels.size());
    m_vVelocity = aMoveVels[static_cast<int>(eDir)];
    if (glm::length(m_vVelocity) > 0.0f)
    {
        vWorldPos += glm::normalize(m_vVelocity) * m_fSpeedScalar;
    }

    /*******************************************/
    /*        Reset velocity, as it will be    */
    /*        set again on player input        */
    /*******************************************/
    m_vVelocity = glm::vec2(0.0f);
}



void Player::handleInput()
{
    if (!Util::IsAnyKeyPressed())
    {
        eState = CharState::IDLE;
        m_vVelocity = glm::vec2(0.0f);
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        eDir = MoveDir::FORWARD;
        eState = CharState::WALK;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        eDir = MoveDir::BACK;
        eState = CharState::WALK;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {

        /*********************************/
        /*        Handle diagonal        */
        /*********************************/
        eState = CharState::WALK;

        if (m_vVelocity.y < 0)
            eDir = MoveDir::FORWARD_LEFT;
        else if (m_vVelocity.y > 0)
            eDir = MoveDir::BACK_LEFT;
        else
            eDir = MoveDir::LEFT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {

        /*********************************/
        /*        Handle diagonal        */
        /*********************************/
        eState = CharState::WALK;

        if (m_vVelocity.y < 0)
            eDir = MoveDir::FORWARD_RIGHT;
        else if (m_vVelocity.y > 0)
            eDir = MoveDir::BACK_RIGHT;
        else
            eDir = MoveDir::RIGHT;
    }
}
