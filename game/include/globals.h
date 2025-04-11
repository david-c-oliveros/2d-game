#pragma once

#include <SFML/System.hpp>

#include <glm/glm.hpp>

namespace Globals
{
    enum class DebugLevel
    {
        ZERO,
        ONE,
        TWO,
        THREE
    };

    constexpr DebugLevel eDEBUG_LEVEL = DebugLevel::ZERO;

    constexpr sf::Vector2f TILE_SIZE({ 32.0f, 32.0f });
    constexpr glm::vec2 GLM_TILE_SIZE(32.0f);

    constexpr uint32_t TOTAL_ENEMIES = 1;

    constexpr std::array<glm::vec2, 9> aMoveVels = { glm::vec2(  0.0f,  0.0f ),
                                                     glm::vec2(  0.0f, -1.0f ),
                                                     glm::vec2(  1.0f, -1.0f ),
                                                     glm::vec2(  1.0f,  0.0f ),
                                                     glm::vec2(  1.0f,  1.0f ),
                                                     glm::vec2(  0.0f,  1.0f ),
                                                     glm::vec2( -1.0f,  1.0f ),
                                                     glm::vec2( -1.0f,  0.0f ),
                                                     glm::vec2( -1.0f, -1.0f )
    };


    /***********************************/
    /*        Physics Constants        */
    /***********************************/
    constexpr bool bTOP_DOWN = true;
    constexpr float METER = 16.0f * 8.0f;
    constexpr float GRAVITY = bTOP_DOWN ? 0.0f : 9.80665f / METER;
}
