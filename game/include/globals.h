#pragma once

#include "SFML/System.hpp"

namespace Globals
{
    constexpr sf::Vector2f TILE_SIZE({ 32.0f, 32.0f });

    constexpr uint32_t TOTAL_ENEMIES = 10;

    constexpr std::array<glm::vec2, 8> aMoveVels = { glm::vec2(  0.0f, -1.0f ),
                                                     glm::vec2(  1.0f, -1.0f ),
                                                     glm::vec2(  1.0f,  0.0f ),
                                                     glm::vec2(  1.0f,  1.0f ),
                                                     glm::vec2(  0.0f,  1.0f ),
                                                     glm::vec2( -1.0f,  1.0f ),
                                                     glm::vec2( -1.0f,  0.0f ),
                                                     glm::vec2( -1.0f, -1.0f )
    };
}
