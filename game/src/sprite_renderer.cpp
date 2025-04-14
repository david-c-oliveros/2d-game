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


void SpriteRenderer::Draw(sf::Sprite &cSprite, glm::vec2 vPos, const GLShader &cShader)
{
    // TMP
    glm::vec2 vSize(32.0f);

    glm::mat4 model = glm::mat4(1.0f);
    float fRotation = 0.0f;

    // TODO - Figure out where sizing of the sprite should be done
    model = glm::translate(model, glm::vec3(vPos, 0.0f));
//    model = glm::translate(model, glm::vec3(0.5f * vSize.x, 0.5f * vSize.y, 0.0f));
    model = glm::rotate(model, glm::radians(fRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(vSize, 1.0f));

    glm::vec2 vRectPos(Util::convert_vector<glm::vec2>(cSprite.getTextureRect().position));
    glm::vec2 vRectSize(Util::convert_vector<glm::vec2>(cSprite.getTextureRect().size));

    cShader.Bind();

    cShader.SetUniform("u_Model", model);
    cShader.SetUniform("u_TexRectPos", vRectPos);
    cShader.SetUniform("u_TexRectSize", vRectSize);
    sf::Color c = cSprite.getColor();;
    cShader.SetUniform("u_Color", glm::vec4(c.r, c.b, c.g, c.a));
    cShader.SetUniform("u_Texture", (int)cSprite.getTexture().getNativeHandle());

    // TODO - Figure out where textures should be handled (custom class or SFML)

//    sf::Texture::bind(&cSprite.getTexture());
//    glActiveTexture(GL_TEXTURE0);

    Renderer::DrawGL(nQuadVAO, cShader, 6);
//    sf::Texture::bind(&cSprite.getTexture());
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
