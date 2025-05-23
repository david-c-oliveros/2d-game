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
    SpriteRenderer::InitRenderer();

    TimeManager::NewTimer("get_fps_interval", 6);

    RM::LoadShader("../../res/shaders/simple.vert",
                   "../../res/shaders/simple.frag",
                   "", "map_shader");

    RM::LoadShader("../../res/shaders/s.vert",
                   "../../res/shaders/s.frag",
                   "", "color_shader");

    LoadResources();

    shape = sf::RectangleShape({ 1.0f, 1.0f });
    shape.setFillColor(sf::Color(250, 150, 100));
    shape.setOutlineThickness(0.0f);
    shape.setOutlineColor(sf::Color(250, 150, 100));

    Camera::SetViewParams(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2(0.0f, 0.0f));
    Camera::SetZoom(0.5f);
    Camera::EnableFollow();

    UI::AddText("player_position", "Player Coords: " + glm::to_string(m_cPlayer.vWorldPos));
    UI::AddText("cursor_grid_position", "Cursor World Coords: " + glm::to_string(util::convert_vector<glm::ivec2>(GetCursorTile())));
    UI::AddText("cursor_world_position", "Cursor World Coords: " + glm::to_string(util::convert_vector<glm::ivec2>(Renderer::GetCursorWorldPos(Camera::GetView()))));
    UI::AddText("player_anim_interval", "Player last anim: " + std::to_string(m_cPlayer.fAnimInterval));
    UI::AddText("fps", "FPS: ");

    UI::AddButton("My Button");
//    UI::SetButtonCallback(&Game::ButtonPressed, std::string("My Button"));

    Renderer::SetProjectionMatrix("map_shader");
}



void Game::Start()
{
    TimeManager::StartTimer("get_fps_interval");

    while (m_bRunning)
    {
        while (const std::optional event = Renderer::GetWindow().pollEvent())
        {
            handleInputEvent(event);
            Camera::HandleMouseInput(event);
//            UI::HandleInput(event);
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

    UI::mLabels.at("player_position")->SetText("Player position: " + glm::to_string(util::convert_vector<glm::vec2>(m_cPlayer.vWorldPos)));
    UI::mLabels.at("cursor_grid_position")->SetText("Cursor Grid Coords: " + glm::to_string(util::convert_vector<glm::vec2>(GetCursorTile())));
    UI::mLabels.at("player_anim_interval")->SetText("Player last anim: " + std::to_string(m_cPlayer.nDebugTotal));
    UI::mLabels.at("cursor_world_position")->SetText("Cursor World Coords: " + glm::to_string(util::convert_vector<glm::ivec2>(Renderer::GetCursorWorldPos(Camera::GetView()))));

    UI::UpdateButtons(Renderer::GetCursorScreenPos());

    m_cPlayer.Update(m_cMap);

    Camera::UpdateFollow(util::convert_vector<sf::Vector2f>(m_cPlayer.vWorldPos + util::convert_vector<glm::vec2>(m_cPlayer.GetSpriteSize()) / 2.0f));

    for (auto &e : aEntities)
    {
        //if (e == nullptr)
        //    continue;

        e.Update(m_cMap);
    }
}



void Game::Render()
{
    Renderer::Clear(sf::Color(34, 34, 54, 0xff));

    Renderer::SetView(Camera::GetView());
    Renderer::SetViewMatrix("map_shader");

    RenderGameWorld();

    if (Globals::eDEBUG_LEVEL > Globals::DebugLevel::ZERO)
        RenderDebug();

    RenderEntities();

    if (Globals::eDEBUG_LEVEL > Globals::DebugLevel::ZERO)
    {
        m_cPlayer.DrawBoundingBox();
        m_cPlayer.DrawCollider();
    }

    RenderUI();

    Renderer::Display();
}



void Game::RenderGameWorld()
{
    RM::GetShader("map_shader").Bind();
    RM::GetShader("map_shader").SetUniform("u_LightPosition", m_cPlayer.vWorldPos);

    m_cMap.Draw(m_cPlayer.vWorldGridPos, RM::GetShader("map_shader"));
}



void Game::RenderEntities()
{
    /*******************************/
    /*        Draw Entities        */
    /*******************************/
    m_cPlayer.Draw("map_shader");

    for (auto &e : aEntities)
    {
        //if (e == nullptr)
        //    continue;

        e.Draw("map_shader");
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
    sf::Vector2i _vCursorTile(GetCursorTile().x * Globals::TILE_SIZE.x, GetCursorTile().y * Globals::TILE_SIZE.y);
    glm::vec2 vCursorTile = util::convert_vector<glm::vec2>(_vCursorTile);

    RM::GetShader("color_shader").Bind();
    RM::GetShader("color_shader").SetUniform("u_Color", glm::vec4(0.4f, 0.7f, 0.2f, 1.0f));
    SpriteRenderer::Draw(vCursorTile, "color_shader");

    /******************************************/
    /*        Draw Current Player Tile        */
    /******************************************/
    sf::Vector2i _pos = util::convert_vector<sf::Vector2i>(m_cPlayer.vWorldGridPos);
    shape.setPosition(sf::Vector2f(_pos.x * Globals::TILE_SIZE.x, _pos.y * Globals::TILE_SIZE.y));
    shape.setFillColor(sf::Color(50, 100, 50, 100));

    Renderer::Draw(shape);

}



sf::Vector2i Game::GetTileAtPos(glm::vec2 _vPos)
{
    sf::Vector2f vPos = util::convert_vector<sf::Vector2f>(_vPos);
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
    EntityCollection sEC = RM::LoadEntityData("../../res/config/entities.json");

    m_cPlayer = sEC.cPlayer;
    aEntities = sEC.aNpcs;

    // TODO - Handle case where aEntities[0] is not of type Player

    sf::Font font;
    if (!font.openFromFile("../../res/font/Pixel Game.otf"))
        util::Log("ERROR loading font");

    UI::SetDefaultFont(font);
    UI::SetDefaultFontSize(40);

    m_cMap.LoadFromFile("sample map demo.json");
}



//void Game::AddEntity(std::unique_ptr<Entity> _pE)
//{
//    for (auto &entity : aEntities)
//    {
//        if (entity == nullptr)
//        {
//            entity = std::move(_pE);
//            break;
//        }
//    }
//}



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
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
        {
            m_bDebug = !m_bDebug;
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
