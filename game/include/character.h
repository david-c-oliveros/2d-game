#pragma once

#include <AnimationManager.hpp>

#include "globals.h"
#include "renderer.h"
#include "entity.h"
#include "geometry.h"


class Map;
class Collision;



/*********************************************/
/*        Goes in clockwise direction        */
/*********************************************/
enum class MoveDir
{
    IDLE,
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
        Character();
        Character(uint32_t _ID, glm::vec2 _vWorldPos);

        Character(uint32_t _ID, std::string _sName, glm::vec2 _vWorldPos);
        ~Character();

        std::shared_ptr<sf::Texture> GetTexture();

        void SetPosition(glm::vec2 _vWorldPos);
        void Update();
        void Move(Map &cMap);
        void Draw(const std::string sShader);
        void DrawBoundingBox();
        void DrawCollider();


        void AttachAnimatedSprite(const std::string _sSpriteName,
                                  const std::string sFilepath,
                                  glm::ivec2 _vSpriteSize,
                                  glm::ivec2 _vFrameRect);

        void AddAnimation(const std::string _sName,
                          glm::ivec2 _vStartIndex,
                          glm::ivec2 _vEndIndex);

        void SetAnimationFrequency(std::string _sName, int32_t _nFreq);
        void SetCurrentAnimation(std::string _sName);

        std::string GetCurrentAnimation();
        sf::Vector2i GetSpriteSize();
        glm::vec2 GetScale();


    protected:
        void setVelocity(glm::vec2 _vVel, float fScalar);
        void updateSprite();
        void updateBoundingBox();
        void setAnimation();


    public:
        std::string sName;

        sf::Clock clock;
        int nDebugCount;
        int nDebugTotal;
        float fAnimInterval;

        bool bAnimated = false;

        geometry::Circle cCollider{ .vPos = vWorldPos / Globals::GLM_TILE_SIZE,
                                    .fRadius = 0.003125f * Globals::GLM_TILE_SIZE.x };


    protected:
        std::shared_ptr<GLSprite> m_pSprite;
//        std::unique_ptr<sf::Texture> m_pTexture;

        std::string m_sCurrentAnimation;
        std::string m_sPreviousAnimation;

        /**********************************************************/
        /*        Note: Sheet size is in indices, so              */
        /*        m_vSheetSize.x is the sprite image              */
        /*        size in pixels divided by m_SpriteSize.x        */
        /**********************************************************/
        sf::Vector2i m_vSpriteSize;
        sf::Vector2i m_vSheetSize;

        CharState eState;
        MoveDir eDir;

        sf::FloatRect cBox;
        glm::vec2 m_vScale = Globals::GLM_TILE_SIZE;
        glm::vec2 m_vVelocity = glm::vec2(0.0f);
        glm::vec2 m_vGravVel = glm::vec2(0.0f);
        glm::vec2 m_vDefaultScalar = glm::vec2(1.0f);
};
