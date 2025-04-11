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



void Player::Update(Map &cMap)
{
    handleInput();

    Character::Update();
    Move(cMap);
}



void Player::Move(Map &cMap)
{
    /*********************************/
    /*        Update position        */
    /*********************************/
    assert(static_cast<int>(eDir) < Globals::aMoveVels.size());
    setVelocity(Globals::aMoveVels[static_cast<int>(eDir)], m_fSpeedScalar);
    Character::Move(cMap);

    /***********************************************/
    /*        Reset velocity, as it will be        */
    /*        set again on player input            */
    /***********************************************/

    /********************************************/
    /*        Update world grid position        */
    /********************************************/
    updateWorldGridPosition();
}



void Player::handleInput()
{
    if (!Util::IsAnyKeyPressed())
    {
        eState = CharState::IDLE;
        eDir = MoveDir::IDLE;
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



void Player::updateWorldGridPosition()
{
    vWorldGridPos.x = vWorldPos.x < 0.0f ?  (int32_t)(vWorldPos.x / Globals::TILE_SIZE.x - 1) : (int32_t)(vWorldPos.x) / Globals::TILE_SIZE.x;
    vWorldGridPos.y = vWorldPos.y < 0.0f ?  (int32_t)(vWorldPos.y / Globals::TILE_SIZE.y - 1) : (int32_t)(vWorldPos.y) / Globals::TILE_SIZE.y;
}
