#pragma once

#include <glm/glm.hpp>


class SpriteRenderer
{
    public:
        static void Draw(glm::vec2 vPos, glm::vec2 vSize);


    private:
        static void initRenderData();


    private:
        static uint32_t nQuadVAO;
};
