#pragma once

#include <glm/glm.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "globals.h"
#include "util.h"
#include "shader.h"
#include "resource_manager.h"
#include "renderer.h"
#include "sprite.h"


class SpriteRenderer
{
    public:
        static void InitRenderer();
        static void Draw(glm::vec2 vPos, const std::string sShader);
        static void Draw(GLSprite &cSprite, const std::string sShader);


    private:
        static void initRenderData();


    public:
        static uint32_t nQuadVAO;
        static uint32_t nQuadVBO;
};
