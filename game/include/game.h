#pragma once

#include <iostream>
#include <vector>
#include <cstddef>
#include <random>

#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glm/gtx/string_cast.hpp>
#include <AnimationManager.hpp>

#include "gl_error_manager.h"
#include "globals.h"
#include "util.h"
#include "shader.h"
#include "renderer.h"
#include "sprite_renderer.h"
#include "entity.h"
#include "character.h"
#include "npc.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "ui.h"
#include "resource_manager.h"


//typedef struct
//{
//    void (Game::*ButtonPressed) (std::string);
//} MemberFuncPtr;

void Log(const std::string sStr);


class Game
{
    public:
        Game(int32_t _nCanvasWidth, int32_t _nCanvasHeight);
        ~Game();

        void Test();
        void OtherTest();

        void Create();
        void Start();
        void Update();
        void Render();
        void RenderGameWorld();
        void RenderEntities();
        void RenderUI();
        void RenderDebug();

        sf::Vector2i GetTileAtPos(glm::vec2 _vPos);
        sf::Vector2i GetCursorTile();

        void EnableFollowCamera();
        void LerpCamera(glm::vec2 _vTarget);
        void ZoomViewAt(sf::Vector2i vPixel, float fZoom);

        glm::mat4 LookAtFromSFView();

        void LoadResources();
        int LoadShaders(const std::filesystem::path &fsVertPath,
                        const std::filesystem::path &fsFragPath);
        void AddEntity(std::unique_ptr<Entity> _pE);

//        void ButtonPressed(std::string sButtonName);

    private:
        void handleInputEvent(std::optional<sf::Event> event);
        uint32_t getNewID();

    public:
        sf::RectangleShape shape;
        sf::Texture pSpritesheet;
        sf::Clock cClock;

        std::unique_ptr<sf::Text> cCurrentSpriteFrame;
        std::unique_ptr<sf::Sprite> pSprite;

        std::vector<Npc> aEntities;
        //std::array<std::unique_ptr<Entity>, Globals::TOTAL_ENEMIES> aEntities = { nullptr };

    private:
        bool m_bRunning = true;

        Player m_cPlayer;
        std::unique_ptr<Player> m_pPlayer;

        GLSprite cDebugSprite;
        Map m_cMap;

        sf::Shader m_shader;

        uint8_t m_nFrameCount = 1;
        uint32_t m_nCurrentID = 0;

        uint32_t nQuadVAO;
        bool m_bDebug = false;;
};
