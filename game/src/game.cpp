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
    m_pPlayer = std::make_unique<Player>(getNewID(), "Player", glm::vec2(0.0f));
    TimeManager::NewTimer("get_fps_interval", 6);

    LoadResources();
    std::cout << "Number of npc's: " << aEntities.size() << std::endl;
    shape = sf::RectangleShape({ 1.0f, 1.0f });
    shape.setFillColor(sf::Color(250, 150, 100));
    shape.setOutlineThickness(0.0f);
    shape.setOutlineColor(sf::Color(250, 150, 100));

    Camera::SetCameraView(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2(960.0f, 540.0f));
    Camera::SetZoom(cWindow, 0.2f);
//    Camera::EnableFollow();

    UI::AddText("player_position", "Player Coords: " + glm::to_string(m_pPlayer->vWorldPos));
    UI::AddText("cursor_grid_position", "Cursor World Coords: " + glm::to_string(Util::convert_vector<glm::ivec2>(GetCursorTile())));
    UI::AddText("fps", "FPS: ");
    UI::AddButton("My Button");
//    UI::SetButtonCallback(&Game::ButtonPressed, std::string("My Button"));
}



void Game::Start()
{
    TimeManager::StartTimer("get_fps_interval");
    while (cWindow.isOpen())
    {
        while (const std::optional event = cWindow.pollEvent())
        {
            handleInputEvent(event);
            cWindow.setView(Camera::cView);
            Camera::HandleMouseInput(cWindow, event);
            UI::HandleInput(cWindow, event);
        }

        Update();
        Render();
    }
}



void Game::Update()
{
    TimeManager::Update();
    if (TimeManager::CheckTimer("get_fps_interval"))
    {
        TimeManager::TimerTimeout("get_fps_interval");
        UI::mLabels["fps"]->SetText("FPS: " + std::to_string(TimeManager::GetFPS()));
    }

    UI::mLabels["player_position"]->SetText("Player position: " + glm::to_string(Util::convert_vector<glm::vec2>(m_pPlayer->vWorldPos)));
    UI::mLabels["cursor_grid_position"]->SetText("Cursor Grid Coords: " + glm::to_string(Util::convert_vector<glm::vec2>(GetCursorTile())));

    UI::UpdateButtons(GetCursorScreenPos());
    m_pPlayer->Update(m_cMap);
    Camera::UpdateFollow(Util::convert_vector<sf::Vector2f>(m_pPlayer->vWorldPos + Util::convert_vector<glm::vec2>(m_pPlayer->GetSpriteSize()) / 2.0f));

    for (auto &e : aEntities)
    {
        if (e == nullptr)
            continue;

        e->Update();
    }
}



void Game::Render()
{
    cWindow.clear(sf::Color::Black);

    RenderGameWorld();
    RenderUI();

    cWindow.display();
}



void Game::RenderGameWorld()
{
    cWindow.setView(Camera::cView);
    m_cMap.Draw(cWindow, m_pPlayer->vWorldGridPos);

    /***************************************/
    /*        Draw Highlighted Tile        */
    /***************************************/
    sf::Vector2i _vCursorTile(GetCursorTile().x * Globals::TILE_SIZE.x, GetCursorTile().y * Globals::TILE_SIZE.y);

    shape.setOutlineColor(sf::Color(150, 150, 100));
    shape.setFillColor(sf::Color(50, 50, 100));

    shape.setPosition(sf::Vector2f(_vCursorTile));
    shape.setScale(Globals::TILE_SIZE);

    cWindow.draw(shape);


    /*******************************/
    /*        Draw Entities        */
    /*******************************/
//    sf::Vector2i _pos = Util::convert_vector<sf::Vector2i>(m_pPlayer->vWorldGridPos);
//    shape.setPosition(sf::Vector2f(_pos.x * Globals::TILE_SIZE.x, _pos.y * Globals::TILE_SIZE.y));
//    shape.setFillColor(sf::Color(50, 100, 50));
//    cWindow.draw(shape);
    m_pPlayer->Draw(cWindow);

    for (auto &e : aEntities)
    {
        if (e == nullptr)
            continue;

        e->Draw(cWindow);
    }
}



void Game::RenderUI()
{
    cWindow.setView(cWindow.getDefaultView());

    UI::Render(cWindow);
}



sf::Vector2i Game::GetCursorScreenPos()
{
    sf::Vector2i _vCursorScreen = sf::Mouse::getPosition(cWindow);
    return _vCursorScreen;
}



sf::Vector2f Game::GetCursorWorldPos()
{
    cWindow.setView(Camera::cView);
    sf::Vector2f _vCursorWorld  = cWindow.mapPixelToCoords(sf::Mouse::getPosition(cWindow));
    return _vCursorWorld;
}



sf::Vector2i Game::GetTileAtPos(glm::vec2 _vPos)
{
    sf::Vector2f vPos = Util::convert_vector<sf::Vector2f>(_vPos);
    sf::Vector2i vTileAtPos({ 0, 0 });
    vTileAtPos.x = vPos.x < 0.0f ?  (int32_t)(vPos.x / Globals::TILE_SIZE.x - 1) : (int32_t)(vPos.x) / Globals::TILE_SIZE.x;
    vTileAtPos.y = vPos.y < 0.0f ?  (int32_t)(vPos.y / Globals::TILE_SIZE.y - 1) : (int32_t)(vPos.y) / Globals::TILE_SIZE.y;

    return vTileAtPos;
}



sf::Vector2i Game::GetCursorTile()
{
    sf::Vector2f _vCursorPos = GetCursorWorldPos();
    sf::Vector2i vCursorTile({ 0, 0 });
    vCursorTile.x = _vCursorPos.x < 0.0f ?  (int32_t)(_vCursorPos.x / Globals::TILE_SIZE.x - 1) : (int32_t)(_vCursorPos.x) / Globals::TILE_SIZE.x;
    vCursorTile.y = _vCursorPos.y < 0.0f ?  (int32_t)(_vCursorPos.y / Globals::TILE_SIZE.y - 1) : (int32_t)(_vCursorPos.y) / Globals::TILE_SIZE.y;

    return vCursorTile;
}



void Game::LoadResources()
{
    sf::Font font;
    if (!font.openFromFile("../../res/font/Pixel Game.otf"))
        std::cout << "ERROR loading font" << std::endl;

    UI::SetDefaultFont(font);
    UI::SetDefaultFontSize(40);

    m_cMap.LoadFromFile("sample map demo.json");

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


    for (size_t i = 0; i < Globals::TOTAL_ENEMIES; i++)
    {
        std::unique_ptr<Npc> _c = std::make_unique<Npc>(getNewID(), "Enemy", glm::vec2(8, 8));
        _c->AttachAnimatedSprite("../../res/pipoya/Enemy 01-1.png", glm::ivec2(32, 32), glm::ivec2(3, 4));

        _c->AddAnimation("walk_back", glm::ivec2(0, 0), glm::ivec2(3, 0));
        _c->AddAnimation("walk_left", glm::ivec2(0, 1), glm::ivec2(3, 1));
        _c->AddAnimation("walk_right", glm::ivec2(0, 2), glm::ivec2(3, 2));
        _c->AddAnimation("walk_forward", glm::ivec2(0, 3), glm::ivec2(3, 3));

        _c->SetAnimationFrequency("walk_back", 8);
        _c->SetAnimationFrequency("walk_left", 8);
        _c->SetAnimationFrequency("walk_right", 8);
        _c->SetAnimationFrequency("walk_forward", 8);

        _c->SetCurrentAnimation("walk_forward");

        AddEntity(std::move(_c));
    }
}



void Game::AddEntity(std::unique_ptr<Entity> _pE)
{
    for (auto &entity : aEntities)
    {
        if (entity == nullptr)
        {
            entity = std::move(_pE);
            break;
        }
    }
}



void Game::handleInputEvent(std::optional<sf::Event> event)
{
    sf::Vector2i vCursorPos = GetCursorScreenPos();

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
    }
}



uint32_t Game::getNewID()
{
    return m_nCurrentID++;
}



//void Game::ButtonPressed(std::string sButtonName)
//{
//    std::cout << sButtonName << " pressed\n";
//}
