#include "player.h"



Player::Player(glm::vec2 _vWorldPos)
    : Character(_vWorldPos)
{
}



Player::~Player()
{
}



void Player::Update()
{
    Character::Update();
    handleInput();

    if (nFrameCount < 1)
    {
        nFrameCount++;
        return;
    }

    if (glm::length(m_vVelocity) > 0.0f)
    {
        vWorldPos += (float)nFrameCount * (glm::normalize(m_vVelocity) * m_speedScalar);
    }

    nFrameCount = 0;
    m_vVelocity = glm::vec2(0.0f);
}



void Player::handleInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        m_vVelocity.y -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        m_vVelocity.y += 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        m_vVelocity.x -= 1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        m_vVelocity.x += 1.0f;
    }
}
