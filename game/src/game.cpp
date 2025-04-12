#include "game.h"



Game::Game(int32_t _nCanvasWidth, int32_t _nCanvasHeight)
{
    /*******************************/
    /*        Create Window        */
    /*******************************/
    sf::Vector2u vDim = { _nCanvasWidth, _nCanvasHeight };
    Renderer::CreateWindow("Cozy Cryptids", vDim, sf::State::Windowed);
}



Game::~Game() {}



void Game::Create()
{
    m_pPlayer = std::make_unique<Player>(getNewID(), "Player", glm::vec2(8, 4));
    TimeManager::NewTimer("get_fps_interval", 6);

    LoadResources();
    LoadShaders("../../res/shaders/simple.vert",
                "../../res/shaders/simple.frag");

    shape = sf::RectangleShape({ 1.0f, 1.0f });
    shape.setFillColor(sf::Color(250, 150, 100));
    shape.setOutlineThickness(0.0f);
    shape.setOutlineColor(sf::Color(250, 150, 100));

    Camera::SetViewParams(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2(960.0f, 540.0f));
    Camera::SetZoom(0.5f);
    Camera::EnableFollow();

    UI::AddText("player_position", "Player Coords: " + glm::to_string(m_pPlayer->vWorldPos));
    UI::AddText("cursor_grid_position", "Cursor World Coords: " + glm::to_string(Util::convert_vector<glm::ivec2>(GetCursorTile())));
    UI::AddText("player_anim_interval", "Player last anim: " + std::to_string(m_pPlayer->fAnimInterval));
    UI::AddText("fps", "FPS: ");

    UI::AddButton("My Button");
//    UI::SetButtonCallback(&Game::ButtonPressed, std::string("My Button"));
}



void Game::Start()
{
    TimeManager::StartTimer("get_fps_interval");

    while (m_bRunning)
    {
        while (const std::optional event = Renderer::GetWindow().pollEvent())
        {
            handleInputEvent(event);
            Renderer::SetView(Camera::GetView());
            Camera::HandleMouseInput(event);
            UI::HandleInput(event);
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
    UI::mLabels["player_anim_interval"]->SetText("Player last anim: " + std::to_string(m_pPlayer->nDebugTotal));

    UI::UpdateButtons(Renderer::GetCursorScreenPos());
    m_pPlayer->Update(m_cMap);
    Camera::UpdateFollow(Util::convert_vector<sf::Vector2f>(m_pPlayer->vWorldPos + Util::convert_vector<glm::vec2>(m_pPlayer->GetSpriteSize()) / 2.0f));

    for (auto &e : aEntities)
    {
        if (e == nullptr)
            continue;

        e->Update(m_cMap);
    }
}



void Game::Render()
{
    Renderer::Clear(sf::Color::Black);

    RenderGameWorld();

    if (Globals::eDEBUG_LEVEL > Globals::DebugLevel::ZERO)
        RenderDebug();

    RenderEntities();

    if (Globals::eDEBUG_LEVEL > Globals::DebugLevel::ZERO)
    {
        //m_pPlayer->DrawBoundingBox();
        m_pPlayer->DrawCollider();
    }

    RenderUI();

    Renderer::Display();
}



void Game::RenderGameWorld()
{
    sf::Transform tView = Camera::GetView().getTransform();
    Renderer::SetView(Camera::GetView());
    m_cMap.Draw(m_pPlayer->vWorldGridPos, tView, m_shader);
}



void Game::RenderEntities()
{
    Renderer::SetView(Camera::GetView());

    /*******************************/
    /*        Draw Entities        */
    /*******************************/
    m_pPlayer->Draw();

    for (auto &e : aEntities)
    {
        if (e == nullptr)
            continue;

        e->Draw();
    }
}



void Game::RenderUI()
{
    Renderer::SetDefaultView();

    UI::Render();
}



void Game::RenderDebug()
{
    /***********************/
    /***********************/
    /*                     */
    /*        DEBUG        */
    /*                     */
    /***********************/
    /***********************/

    /**********************************/
    /*        Draw Cursor Tile        */
    /**********************************/
    sf::Vector2i _vCursorTile(GetCursorTile().x * Globals::TILE_SIZE.x, GetCursorTile().y * Globals::TILE_SIZE.y);

    shape.setOutlineColor(sf::Color(150, 150, 100));
    shape.setFillColor(sf::Color(50, 50, 100));

    shape.setPosition(sf::Vector2f(_vCursorTile));
    shape.setScale(Globals::TILE_SIZE);

    Renderer::Draw(shape);

    /******************************************/
    /*        Draw Current Player Tile        */
    /******************************************/
    sf::Vector2i _pos = Util::convert_vector<sf::Vector2i>(m_pPlayer->vWorldGridPos);
    shape.setPosition(sf::Vector2f(_pos.x * Globals::TILE_SIZE.x, _pos.y * Globals::TILE_SIZE.y));
    shape.setFillColor(sf::Color(50, 100, 50, 100));

    Renderer::Draw(shape);

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
    sf::Vector2f _vCursorPos = Renderer::GetCursorWorldPos(Camera::GetView());
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

    std::cout << "- Current player animation: " << m_pPlayer->GetCurrentAnimation() << '\n';


    for (size_t i = 0; i < Globals::TOTAL_ENEMIES; i++)
    {
        std::unique_ptr<Npc> _c = std::make_unique<Npc>(getNewID(), "Enemy", glm::vec2(4, 2));
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



int Game::LoadShaders(const std::filesystem::path &fsVertPath,
                      const std::filesystem::path &fsFragPath)
{
    if (!sf::Shader::isAvailable())
    {
        std::cout << "ERROR::Shaders not available\n";
        return -1;
    }
    std::cout << "- Shaders available\n";

    if (!m_shader.loadFromFile(fsVertPath, sf::Shader::Type::Vertex))
    {
        std::cout << "ERROR::Could not load vertex shader\n";
        return -1;
    }
    std::cout << "- Vertex shader loaded succesfully\n";

    if (!m_shader.loadFromFile(fsFragPath, sf::Shader::Type::Fragment))
    {
        std::cout << "ERROR::Could not load fragment shader\n";
        return -1;
    }
    std::cout << "- Fragment shader loaded succesfully\n";

    return 0;
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
    if (event->is<sf::Event::Closed>())
    {
        m_bRunning = false;
    }
    else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            m_bRunning = false;
        }
    }
    else if (const auto resized = event->getIf<sf::Event::Resized>())
    {
        Renderer::OnWindowResize(resized->size);
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
