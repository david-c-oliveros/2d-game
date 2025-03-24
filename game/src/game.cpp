#include "game.h"



Game::Game(int32_t _nCanvasWidth, int32_t _nCanvasHeight)
{
    /*******************************/
    /*        Create Window        */
    /*******************************/
    sf::Vector2u vDim = { _nCanvasWidth, _nCanvasHeight };
    cWindow = sf::RenderWindow(sf::VideoMode(vDim), "SFML window", sf::Style::Default, sf::State::Windowed);
    cWindow.setVerticalSyncEnabled(true);
    cWindow.setPosition((sf::Vector2<int>)vDim - (sf::Vector2<int>)vDim / 2);
}



Game::~Game() {}



void Game::Create()
{
    m_pPlayer = std::make_unique<Player>(glm::vec2(0.0f));

    LoadResources();
    shape = sf::RectangleShape({ 1.0f, 1.0f });
    shape.setFillColor(sf::Color(250, 150, 100));
    shape.setOutlineThickness(0.0f);
    shape.setOutlineColor(sf::Color(250, 150, 100));

    cCamera = sf::View(sf::FloatRect({ 0.0f, 0.0f }, { 1920.0f, 1080.0f }));
    cWindow.setView(cCamera);

    cTileWorld.Create(glm::ivec2(64, 64), glm::vec2(100.0f));

    cText = std::make_unique<sf::Text>(font, "Hello SFML", 50);
    cOtherText = std::make_unique<sf::Text>(font, "Hello SFML", 50);
    cOtherText->setPosition({ 0.0f, 100.0f });
}



void Game::Start()
{
    while (cWindow.isOpen())
    {
        while (const std::optional event = cWindow.pollEvent())
        {
            handleInputEvent(event);
        }

        Update();
        Render();
    }
}



void Game::Update()
{
    cText->setString(glm::to_string(GetHoveredTile()));
    m_pPlayer->Update();

    for (auto e : vecEntities)
    {
        e->Update();
    }
}



void Game::Render()
{
    cWindow.clear(sf::Color::Black);

    pMap.Draw(cWindow, cTileWorld);

    glm::vec2 _vScalar = cTileWorld.GetWorldScale();
    shape.setScale(Util::glm_to_sf_vec2(_vScalar));
    for (int32_t i = 0; i < 1;  i++)
    {
        for (int32_t j = 0; j < 1; j++)
        {
            /****************************************/
            /*        Set Position and Scale        */
            /****************************************/
            sf::Vector2f _vWorldPos(i, j);
//            glm::vec2 _vScreenPos = cTileWorld.WorldToScreen(_vWorldPos);


            shape.setPosition(Util::glm_to_sf_vec2(_vScreenPos));

            cWindow.draw(shape);
        }
    }

    /***************************************/
    /*        Draw Highlighted Tile        */
    /***************************************/
    glm::ivec2 _vCursorTile = GetHoveredTile();
    if (_vCursorTile.x >= 0 && _vCursorTile.x < 64 &&
        _vCursorTile.y >= 0 && _vCursorTile.y < 64)
    {
        shape.setOutlineColor(sf::Color(150, 150, 100));
        shape.setFillColor(sf::Color(150, 150, 100));
        shape.setScale(Util::glm_to_sf_vec2(_vScalar));

        glm::vec2 _vScreenPos = cTileWorld.WorldToScreen(GetHoveredTile());
        shape.setPosition(Util::glm_to_sf_vec2(_vScreenPos));
        cWindow.draw(shape);

        shape.setOutlineColor(sf::Color(250, 150, 100));
    }

    m_pPlayer->Draw(cWindow, cTileWorld);

    for (auto &e : vecEntities)
    {
        e->Draw(cWindow, cTileWorld);
    }

    cWindow.draw(*cText);
    cWindow.draw(*cOtherText);

    cWindow.display();
}



glm::vec2 Game::GetCursorScreenPos()
{
    sf::Vector2i _vCursorPos = sf::Mouse::getPosition(cWindow);
    return glm::vec2(_vCursorPos.x, _vCursorPos.y);
}



glm::vec2 Game::GetCursorWorldPos()
{
    return cTileWorld.ScreenToWorld(GetCursorScreenPos());
}



glm::ivec2 Game::GetHoveredTile()
{
    glm::vec2 _vHoveredTileFloat = GetCursorWorldPos();
    glm::ivec2 vHoveredTile;
    vHoveredTile.x = _vHoveredTileFloat.x < 0.0 ? (int32_t)(_vHoveredTileFloat.x - 1) : (int32_t)(_vHoveredTileFloat.x);
    vHoveredTile.y = _vHoveredTileFloat.y < 0.0 ? (int32_t)(_vHoveredTileFloat.y - 1) : (int32_t)(_vHoveredTileFloat.y);

    return vHoveredTile;
}



void Game::EnableFollowCamera()
{
    sf::Vector2u _vScreenSize = cWindow.getSize();
    glm::vec2 vCenter = cTileWorld.ScreenToWorld(glm::vec2(_vScreenSize.x, _vScreenSize.y));
    cTileWorld.StartPan(vCenter);
}



void Game::LerpCamera(glm::vec2 _vTarget)
{
    sf::Vector2u _vScreenSize = cWindow.getSize();
    glm::vec2 vCenter = cTileWorld.ScreenToWorld(glm::vec2(_vScreenSize.x, _vScreenSize.y));
    glm::vec2 vDelta = glm::mix(vCenter, _vTarget, 0.5f);
    cOtherText->setString(glm::to_string(cTileWorld.GetWorldOffset()));
    vDelta = -vDelta;
    cTileWorld.UpdatePan(vDelta);
}



void Game::LoadResources()
{
    if (!font.openFromFile("../../res/font/Pixel Game.otf"))
        std::cout << "ERROR loading font" << std::endl;

//    pMap.LoadFromFile("../../res/tilemaps/rpg_world_pack_demo/Tilesets and props/rpg_demo.json");
    pMap.LoadFromFile("sample map demo.json");
    m_pPlayer->AttachAnimatedSprite("../../res/pipoya/Male 09-1.png", glm::ivec2(32, 32), glm::ivec2(3, 4));

    m_pPlayer->AddAnimation("walk_back", glm::ivec2(0, 0), glm::ivec2(3, 0));
    m_pPlayer->AddAnimation("walk_left", glm::ivec2(0, 1), glm::ivec2(3, 1));
    m_pPlayer->AddAnimation("walk_right", glm::ivec2(0, 2), glm::ivec2(3, 2));
    m_pPlayer->AddAnimation("walk_forward", glm::ivec2(0, 3), glm::ivec2(3, 3));

    m_pPlayer->SetAnimationFrequency("walk_back", 8);
    m_pPlayer->SetAnimationFrequency("walk_left", 8);
    m_pPlayer->SetAnimationFrequency("walk_right", 8);
    m_pPlayer->SetAnimationFrequency("walk_forward", 8);

    m_pPlayer->SetCurrentAnimation("walk_right");

//    std::shared_ptr<Character> _c = std::make_shared<Character>(glm::vec2(0, 0));
//    _c->AttachAnimatedSprite("../../res/pipoya/Male 09-1.png",
//                                    glm::ivec2(32, 32), glm::ivec2(3, 4));
//
//    _c->AddAnimation("walking", glm::ivec2(0, 0), glm::ivec2(4, 0));
//    _c->SetAnimationFrequency("walking", 8);
//    _c->SetCurrentAnimation("walking");
//
//    vecEntities.push_back(_c);
}



void Game::handleInputEvent(std::optional<sf::Event> event)
{
    glm::vec2 vCursorPos = GetCursorScreenPos();

    if (event->is<sf::Event::Closed>())
    {
        cWindow.close();
    }
    else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            cWindow.close();
        }

        else if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            m_pPlayer->Update();
        }
    }

    /*********************/
    /*        Pan        */
    /*********************/
    else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
        {
            if (!m_bPanning)
            {
                cTileWorld.StartPan(vCursorPos);
                m_bPanning = true;
            }
        }
    }
    else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
        {
            if (m_bPanning)
            {
                cTileWorld.EndPan(vCursorPos);
                m_bPanning = false;
            }
        }
    }

    else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
    {
        if (m_bPanning)
        {
            cTileWorld.UpdatePan(vCursorPos);
        }
    }

    /**********************/
    /*        Zoom        */
    /**********************/
    else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
    {
        sf::Vector2i _vCursorLocalPosition = sf::Mouse::getPosition(cWindow);
        glm::ivec2 vCursorLocalPosition = glm::ivec2(_vCursorLocalPosition.x, _vCursorLocalPosition.y);
        if (mouseWheelScrolled->delta < 0)
            cTileWorld.ZoomAtScreenPos(0.5f, vCursorLocalPosition);
        else
            cTileWorld.ZoomAtScreenPos(2.0f, vCursorLocalPosition);
    }
}
