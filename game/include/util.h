#pragma once

#include <SFML/System.hpp>
#include <glm/glm.hpp>



namespace Util
{
    template <typename T, typename U>
    static T convert_vector(const U& _v)
    {
        return T(_v.x, _v.y);
    }
}
