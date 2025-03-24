#pragma once

#include <AnimationManager.hpp>

#include "util.h"
#include "entity.h"



class Character : public Entity
{
    public:
        Character(glm::vec2 _vWorldPos);
        ~Character();

        std::shared_ptr<sf::Texture> GetTexture();

        void Update();
        void Draw(sf::RenderWindow &cWindow, TileWorld &cTileWorld);


        void AttachAnimatedSprite(std::string _sFilepath,
                                  glm::ivec2 _vSpriteSize,
                                  glm::ivec2 _vFrameRect);

        void AddAnimation(std::string _sName,
                          glm::ivec2 _vStartIndex,
                          glm::ivec2 _vEndIndex);

        void SetAnimationFrequency(std::string _sName, int32_t _nFreq);
        void SetCurrentAnimation(std::string _sName);

    protected:
        std::unique_ptr<sf::Texture> m_pTexture;
        std::unique_ptr<sf::Sprite> m_pSprite;
        std::string m_sCurrentAnimation;

        sf::Vector2i m_vSpriteSize;
        sf::Vector2i m_vFrameRect;

        bool bAnimated = false;

        glm::vec2 m_vDefaultScalar = glm::vec2(1.0f);
};
