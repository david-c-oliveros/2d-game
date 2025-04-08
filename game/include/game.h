#pragma once

#include <iostream>
#include <vector>
#include <cstddef>
#include <random>

#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <glm/gtx/string_cast.hpp>
#include <AnimationManager.hpp>

#include "globals.h"
#include "util.h"
#include "tile_world.h"
#include "entity.h"
#include "character.h"
#include "npc.h"
#include "player.h"
#include "map.h"
#include "camera.h"
#include "ui.h"


//typedef struct
//{
//    void (Game::*ButtonPressed) (std::string);
//} MemberFuncPtr;


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
        void RenderUI();

        sf::Vector2i GetCursorScreenPos();
        sf::Vector2f GetCursorWorldPos();
        sf::Vector2i GetTileAtPos(glm::vec2 _vPos);
        sf::Vector2i GetCursorTile();

        void EnableFollowCamera();
        void LerpCamera(glm::vec2 _vTarget);
        void ZoomViewAt(sf::Vector2i vPixel, float fZoom);

        void LoadResources();
        void AddEntity(std::unique_ptr<Entity> _pE);

//        void ButtonPressed(std::string sButtonName);

    private:
        void handleInputEvent(std::optional<sf::Event> event);
        uint32_t getNewID();

    public:
        sf::RenderWindow cWindow;
        sf::RectangleShape shape;
        sf::Texture pSpritesheet;
        sf::Clock cClock;

        std::unique_ptr<sf::Text> cCurrentSpriteFrame;
        std::unique_ptr<sf::Sprite> pSprite;

        std::array<std::unique_ptr<Entity>, Globals::TOTAL_ENEMIES> aEntities = { nullptr };

    private:
        bool m_bPanning = false;
        glm::ivec2 m_vHoveredTile;

        std::unique_ptr<Player> m_pPlayer;
        Map m_cMap;

        uint8_t m_nFrameCount = 1;
        uint32_t m_nCurrentID = 0;
};
