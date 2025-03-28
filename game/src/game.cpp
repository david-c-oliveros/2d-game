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

    Camera::SetCameraView(sf::Vector2f(1920.0f, 1080.0f), sf::Vector2(960.0f, 540.0f));
    Camera::SetZoom(cWindow, 0.2f);
    Camera::EnableFollow();

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
            cWindow.setView(Camera::cView);
            Camera::HandleMouseInput(cWindow, event);
        }

        Update();
        Render();
    }
}



void Game::Update()
{
    cText->setString("Camera center: " + glm::to_string(Util::convert_vector<glm::vec2>(Camera::cView.getCenter())));
    cOtherText->setString("Camera size: " + glm::to_string(Util::convert_vector<glm::vec2>(Camera::cView.getSize())));

    m_pPlayer->Update();
    Camera::UpdateFollow(Util::convert_vector<sf::Vector2f>(m_pPlayer->vWorldPos + Util::convert_vector<glm::vec2>(m_pPlayer->GetSpriteSize()) / 2.0f));

    for (auto e : vecEntities)
    {
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
    m_pMap.Draw(cWindow);

    for (int32_t i = 0; i < 1;  i++)
    {
        for (int32_t j = 0; j < 1; j++)
        {
            /****************************************/
            /*        Set Position and Scale        */
            /****************************************/
            sf::Vector2f _vWorldPos(i, j);

            cWindow.draw(shape);
        }
    }

    /***************************************/
    /*        Draw Highlighted Tile        */
    /***************************************/
    sf::Vector2i _vCursorTile = GetHoveredTile();
    if (true)
    {
        shape.setOutlineColor(sf::Color(150, 150, 100));
        shape.setFillColor(sf::Color(150, 150, 100));

        sf::Vector2i _vScreenPos = cWindow.mapCoordsToPixel(sf::Vector2f(GetHoveredTile().x, GetHoveredTile().y));
        shape.setPosition(sf::Vector2f(_vScreenPos));
        cWindow.draw(shape);

        shape.setOutlineColor(sf::Color(250, 150, 100));
    }

    m_pPlayer->Draw(cWindow);

    for (auto &e : vecEntities)
    {
        e->Draw(cWindow);
    }
}



void Game::RenderUI()
{
    cWindow.setView(cWindow.getDefaultView());

    cWindow.draw(*cText);
    cWindow.draw(*cOtherText);
}



sf::Vector2i Game::GetCursorScreenPos()
{
    sf::Vector2i _vCursorScreen = sf::Mouse::getPosition(cWindow);
    return _vCursorScreen;
}



sf::Vector2f Game::GetCursorWorldPos()
{
    sf::Vector2f _vCursorWorld  = cWindow.mapPixelToCoords(sf::Mouse::getPosition(cWindow));
    return _vCursorWorld;
}



// TODO - This is broken
sf::Vector2i Game::GetHoveredTile()
{
    sf::Vector2f _vHoveredTileFloat = GetCursorWorldPos();
    sf::Vector2i vHoveredTile;
    vHoveredTile.x = _vHoveredTileFloat.x < 0.0 ? (int32_t)(_vHoveredTileFloat.x - 1) : (int32_t)(_vHoveredTileFloat.x);
    vHoveredTile.y = _vHoveredTileFloat.y < 0.0 ? (int32_t)(_vHoveredTileFloat.y - 1) : (int32_t)(_vHoveredTileFloat.y);

    return vHoveredTile;
}



void Game::LoadResources()
{
    if (!font.openFromFile("../../res/font/Pixel Game.otf"))
        std::cout << "ERROR loading font" << std::endl;

    m_pMap.LoadFromFile("sample map demo.json");
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
