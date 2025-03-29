#include "player.h"



Player::Player(uint32_t _ID, glm::vec2 _vWorldPos)
    : Character(_ID, _vWorldPos)
{
}



Player::Player(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos)
    : Character(_ID, _sName, _vWorldPos)
{
}



Player::~Player()
{
}



void Player::Update()
{
    Character::Update();
    handleInput();

    Move();
}



void Player::Move()
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

    /*********************************/
    /*        Update position        */
    /*********************************/
    if (glm::length(m_vVelocity) > 0.0f)
    {
        vWorldPos += glm::normalize(m_vVelocity) * m_fSpeedScalar;
    }

    m_vVelocity = glm::vec2(0.0f);
}



void Player::handleInput()
{
    uint8_t combo_direction = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        m_vVelocity.y -= 1.0f;
        eDir = MoveDir::FORWARD;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        m_vVelocity.y += 1.0f;
        eDir = MoveDir::BACK;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        m_vVelocity.x -= 1.0f;

        /*********************************/
        /*        Handle diagonal        */
        /*********************************/
        if (m_vVelocity.y < 0)
            eDir = MoveDir::FORWARD_LEFT;
        else if (m_vVelocity.y > 0)
            eDir = MoveDir::BACK_LEFT;
        else
            eDir = MoveDir::LEFT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        m_vVelocity.x += 1.0f;

        /*********************************/
        /*        Handle diagonal        */
        /*********************************/
        if (m_vVelocity.y < 0)
            eDir = MoveDir::FORWARD_RIGHT;
        else if (m_vVelocity.y > 0)
            eDir = MoveDir::BACK_RIGHT;
        else
            eDir = MoveDir::RIGHT;
    }
}
