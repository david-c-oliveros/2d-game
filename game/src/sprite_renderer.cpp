#include "sprite_renderer.h"

#include <glad/glad.h>

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>


uint32_t SpriteRenderer::nQuadVAO;
uint32_t SpriteRenderer::nQuadVBO;


void SpriteRenderer::InitRenderer()
{
    initRenderData();
}


void SpriteRenderer::Draw(glm::vec2 vPos, const std::string sShader)
{
    glm::mat4 model = glm::mat4(1.0f);
    float fRotation = 0.0f;

    model = glm::translate(model, glm::vec3(vPos, 0.0f));
    model = glm::rotate(model, glm::radians(fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(Globals::GLM_TILE_SIZE, 1.0f));

    RM::GetShader(sShader).Bind();
    RM::GetShader(sShader).SetUniform("u_Model", model);

    Renderer::DrawGL(nQuadVAO, RM::GetShader(sShader), 6);
}


void SpriteRenderer::Draw(GLSprite &cSprite, const std::string sShader)
{
    glm::mat4 model = glm::mat4(1.0f);
    float fRotation = 0.0f;

    model = glm::translate(model, glm::vec3(cSprite.GetPosition(), 0.0f));
    model = glm::rotate(model, glm::radians(fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(Globals::GLM_TILE_SIZE, 1.0f));

    glm::vec2 vRectPos(util::convert_vector<glm::vec2>(cSprite.GetTexRectNorm().position));
    glm::vec2 vRectSize(util::convert_vector<glm::vec2>(cSprite.GetTexRectNorm().size));

    RM::GetShader(sShader).Bind();
    RM::GetShader(sShader).SetUniform("u_Model", model);
    RM::GetShader(sShader).SetUniform("u_TexRectPos", vRectPos);
    RM::GetShader(sShader).SetUniform("u_TexRectSize", vRectSize);

    glm::vec4 color = cSprite.GetColor();;
    RM::GetShader(sShader).SetUniform("u_Color", color);
    cSprite.GetTexture().Bind();
    RM::GetShader(sShader).SetUniform("u_Texture", 0);

    Renderer::DrawGL(nQuadVAO, RM::GetShader(sShader), 6);
}


void SpriteRenderer::initRenderData()
{
    uint32_t nVBO;
    float vertices[] =
    {   
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };


    glGenVertexArrays(1, &nQuadVAO);
    glBindVertexArray(nQuadVAO);
    glGenBuffers(1, &nVBO);

    glBindBuffer(GL_ARRAY_BUFFER, nVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),  (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}
