#pragma once

#include <AnimationManager.hpp>

#include "globals.h"
#include "util.h"
#include "map.h"
#include "entity.h"
#include "collision.h"



/*********************************************/
/*        Goes in clockwise direction        */
/*********************************************/
enum class MoveDir
{
    FORWARD,
    FORWARD_RIGHT,
    RIGHT,
    BACK_RIGHT,
    BACK,
    BACK_LEFT,
    LEFT,
    FORWARD_LEFT,
};



enum class CharState
{
    IDLE,
    WALK
};



class Character : public Entity
{
    public:
        Character(uint32_t _ID, glm::vec2 _vWorldPos);

        Character(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos);
        ~Character();

        std::shared_ptr<sf::Texture> GetTexture();

        void Update();
        void Move(Map &cMap);
        void Draw(sf::RenderWindow &cWindow);
        void DrawBoundingBox(sf::RenderWindow &cWindow);
        void DrawCollider(sf::RenderWindow &cWindow);


        void AttachAnimatedSprite(const std::string _sFilepath,
                                  glm::ivec2 _vSpriteSize,
                                  glm::ivec2 _vFrameRect);

        void AddAnimation(const std::string _sName,
                          glm::ivec2 _vStartIndex,
                          glm::ivec2 _vEndIndex);

        void SetAnimationFrequency(std::string _sName, int32_t _nFreq);
        void SetCurrentAnimation(std::string _sName);

        std::string GetCurrentAnimation();
        sf::Vector2i GetSpriteSize();


    protected:
        void setVelocity(glm::vec2 _vVel, float fScalar);
        void updateBoundingBox();
        void setAnimation();


    public:
        std::string sName;

        sf::Clock clock;
        int nDebugCount;
        int nDebugTotal;
        float fAnimInterval;

        bool bAnimated = false;

        Circle cCollider{vWorldPos / Globals::GLM_TILE_SIZE, 0.1f };


    protected:
        std::unique_ptr<sf::Sprite> m_pSprite;
        std::unique_ptr<sf::Texture> m_pTexture;

        std::string m_sCurrentAnimation;
        std::string m_sPreviousAnimation;

        sf::Vector2i m_vSpriteSize;
        sf::Vector2i m_vFrameRect;

        CharState eState;
        MoveDir eDir;

        sf::FloatRect cBox;
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        glm::vec2 m_vDefaultScalar = glm::vec2(1.0f);
};
