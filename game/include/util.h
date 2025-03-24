#pragma once

#include <SFML/System.hpp>
#include <glm/glm.hpp>



namespace Util
{
    static glm::ivec2 sf_to_glm_ivec2(sf::Vector2i _v)
    {
        return glm::ivec2(_v.x, _v.y);
    }



    static glm::vec2 sf_to_glm_vec2(sf::Vector2f _v)
    {
        return glm::vec2(_v.x, _v.y);
    }



    static sf::Vector2i glm_to_sf_ivec2(glm::ivec2 _v)
    {
        return sf::Vector2i(_v.x, _v.y);
    }



    static sf::Vector2f glm_to_sf_vec2(glm::vec2 _v)
    {
        return sf::Vector2(_v.x, _v.y);
    }
}
