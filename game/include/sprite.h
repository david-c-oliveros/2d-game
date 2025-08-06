#pragma once

#include <string>
#include <glm/glm.hpp>
#include <SFML/Graphics/Rect.hpp>


class GLTexture;



class GLSprite
{
    public:
        GLSprite();
        GLSprite(const std::string sTexName);
        ~GLSprite();

        void SetTexture(const std::string sTexName);
        void SetColor(glm::vec4 vColor);
        void SetTextureRect(sf::IntRect _TextureRect);
        void SetPosition(glm::vec2 vPos);
        void SetScale(glm::vec2 &_vScale);

        const sf::Rect<float> GetTexRectNorm() const;
        const glm::vec2& GetTextureSize() const;
        const glm::vec2& GetPosition() const;
        const glm::vec2& GetScale() const;
        const GLTexture GetTexture() const;
        const glm::vec4& GetColor() const;
        const std::string GetTexName() const;


    private:
        std::string m_sTexName;
        glm::vec4 m_vColor;
        glm::vec2 m_vPos;
        glm::vec2 m_vScale;

        /****************************************************************/
        /*        This is in pixel space, and must be normalized        */
        /*        before applying to the sprite's uniform               */
        /****************************************************************/
        sf::Rect<int> m_TextureRect;
};
