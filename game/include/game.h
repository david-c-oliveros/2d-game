#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <glm/gtx/string_cast.hpp>
#include <AnimationManager.hpp>

#include "util.h"
#include "tile_world.h"
#include "entity.h"
#include "character.h"
#include "player.h"
#include "map.h"
#include "camera.h"



#define RECT_SIZE sf::Vector2f(0.9f, 0.9f)
#define RECT_OL_SIZE sf::Vector2f(0.1f, 0.1f)
#define TILE_SIZE glm::vec2(32.0f)


class Game
{
    public:
        Game(int32_t _nCanvasWidth, int32_t _nCanvasHeight);
        ~Game();

        void Create();
        void Start();
        void Update();
        void Render();
        void RenderGameWorld();
        void RenderUI();

        glm::vec2 GetCursorScreenPos();
        glm::vec2 GetCursorWorldPos();
        glm::ivec2 GetHoveredTile();

        void EnableFollowCamera();
        void LerpCamera(glm::vec2 _vTarget);
        void ZoomViewAt(sf::Vector2i vPixel, float fZoom);

        void LoadResources();

    private:
        void handleInputEvent(std::optional<sf::Event> event);

    public:
        sf::RenderWindow cWindow;
        sf::RectangleShape shape;
        sf::Texture pSpritesheet;

        sf::Font font;
        std::unique_ptr<sf::Text> cText;
        std::unique_ptr<sf::Text> cOtherText;
        std::unique_ptr<sf::Text> cCurrentSpriteFrame;
        std::unique_ptr<sf::Sprite> pSprite;

        std::vector<std::shared_ptr<Entity>> vecEntities;

        std::array<glm::ivec2, 64> vecMap;

    private:
        bool m_bPanning = false;
        glm::ivec2 m_vHoveredTile;

        std::unique_ptr<Player> m_pPlayer;
        Map pMap;
};
