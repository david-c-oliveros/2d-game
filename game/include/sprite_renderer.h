#pragma once

#include <glm/glm.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "util.h"
#include "shader.h"
#include "renderer.h"


class SpriteRenderer
{
    public:
        static void InitRenderer();
        static void Draw(sf::Sprite &cSprite, glm::vec2 vPos, const GLShader &cShader);


    private:
        static void initRenderData();


    private:
        static uint32_t nQuadVAO;
        static uint32_t nQuadVBO;
};
