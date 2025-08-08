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

    Renderer::SetProjectionMatrix("map_shader");

    eCurrentScene = Globals::Scene::MENU;
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
            UI::HandleInput(*mScenes[eCurrentScene], event);
        }

        Update();
        Render();
    }
}



void Game::Update()
{
    switch(eCurrentScene)
    {
        case Globals::Scene::MENU:
            UI::UpdateButtons(*mScenes[eCurrentScene], Renderer::GetCursorScreenPos());
            break;

        case Globals::Scene::GAME:
            TimeManager::Update();
            if (TimeManager::CheckTimer("get_fps_interval"))
            {
                TimeManager::TimerTimeout("get_fps_interval");
                mScenes[eCurrentScene]->mLabels["fps"]->SetText("FPS: " + std::to_string(TimeManager::GetFPS()));
            }

            mScenes[eCurrentScene]->mLabels.at("player_position")->SetText("Player position: " + glm::to_string(util::convert_vector<glm::vec2>(m_cPlayer.vWorldPos)));
            mScenes[eCurrentScene]->mLabels.at("npc_position")->SetText("Npc position: " + glm::to_string(util::convert_vector<glm::vec2>(aEntities[0].vWorldPos)));
            mScenes[eCurrentScene]->mLabels.at("camera_center")->SetText("Camera center: " + glm::to_string(util::convert_vector<glm::vec2>(Camera::GetView().getCenter())));
            mScenes[eCurrentScene]->mLabels.at("cursor_grid_position")->SetText("Cursor Grid Coords: " + glm::to_string(util::convert_vector<glm::vec2>(GetCursorTile())));
            mScenes[eCurrentScene]->mLabels.at("cursor_world_position")->SetText("Cursor World Coords: " + glm::to_string(util::convert_vector<glm::ivec2>(Renderer::GetCursorWorldPos(Camera::GetView()))));

            UI::UpdateButtons(*mScenes[eCurrentScene], Renderer::GetCursorScreenPos());
            m_cPlayer.Update(m_cMap);

            Camera::UpdateFollow(util::convert_vector<sf::Vector2f>(m_cPlayer.vWorldPos));

            for (auto &e : aEntities)
            {
                //if (e == nullptr)
                //    continue;

                e.Update(m_cMap);
            }
            break;
    }
}



void Game::Render()
{
    Renderer::Clear(sf::Color(34, 34, 54, 0xff));

    Renderer::SetView(Camera::GetView());
    Renderer::SetViewMatrix("map_shader");

    RM::GetShader("map_shader").Bind();
    RM::GetShader("map_shader").SetUniform("u_LightPosition", m_cPlayer.vWorldPos);

    //RenderMapLayer(0);

    if (m_bDebug)
        RenderDebug();

    RenderEntities();
    RenderStructures();
    //RenderMapLayer(1);

    if (m_bDebug)
    {
        m_cPlayer.DrawBoundingBox();
        m_cPlayer.DrawCollider();
        m_cMap.DrawDebug(m_cPlayer.vWorldGridPos, RM::GetShader("map_shader"), 0);
    }

    RenderUI();

    Renderer::Display();
}



void Game::RenderMapLayer(int32_t nLayer)
{
    m_cMap.Draw(m_cPlayer.vWorldGridPos, RM::GetShader("map_shader"), nLayer);
}



void Game::RenderEntities()
{
    m_cPlayer.Draw("map_shader");

    for (auto &e : aEntities)
    {
        //if (e == nullptr)
        //    continue;

        e.Draw("map_shader");
    }
}



void Game::RenderStructures()
{
    if (m_bDebug)
        m_pBuilding->DrawDebug("map_shader");
    else
        m_pBuilding->Draw("map_shader");
//    for (auto &b : aBuildings)
//    {
//        b.Draw("map_shader");
//    }
}



void Game::RenderUI()
{
    Renderer::SetDefaultView();

    UI::Render(*mScenes[eCurrentScene]);
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
    sf::Vector2i _pos = _vCursorTile;
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
    /******************************/
    /*        Load font(s)        */
    /******************************/
    sf::Font font;
    std::string sFontFile = "../../res/font/Pixel Game.otf";

    if (!font.openFromFile(sFontFile))
        util::Log("ERROR loading font");

    UI::SetDefaultFont(font);
    UI::SetDefaultFontSize(80);

    UI::RegisterCallbacks();

    std::string sGameSceneFile("../../res/config/debug_scene.json");
    std::string sMenuSceneFile("../../res/config/menu.json");
    mScenes[Globals::Scene::MENU] = std::make_unique<Scene>(sMenuSceneFile);
    mScenes[Globals::Scene::GAME] = std::make_unique<Scene>(sGameSceneFile);


    /**********************************************/
    /*        Load entities and structures        */
    /**********************************************/
    CharacterCollection sCC = RM::LoadCharacterData("../../res/config/entities.json");

    m_cPlayer = sCC.cPlayer;
    aEntities = sCC.aNpcs;

    // TODO - Write proper move constructors for children of Entity!
    m_pBuilding = std::make_unique<Building>(RM::GetNewResourceID(), "House", glm::vec2(5, 4));
    m_pBuilding->AttachSprite("house", "../../res/tilemaps/structures/Roofs.png", { 0, 0 }, { 32, 32 });
//    aBuildings.emplace_back(std::move(b));


    /**************************/
    /*        Load Map        */
    /**************************/
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
            util::Log(UI::m_sHoveredButton);
        }
    }
    else if (const auto resized = event->getIf<sf::Event::Resized>())
    {
        Renderer::OnWindowResize(resized->size);
    }
}



//uint32_t Game::getNewID()
//{
//    return m_nCurrentID++;
//}



void Game::ButtonPressed(std::string sButtonName)
{
    std::cout << sButtonName << " pressed\n";
}
