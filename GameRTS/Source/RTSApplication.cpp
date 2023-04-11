#include <gePrerequisitesUtil.h>

#if GE_PLATFORM == GE_PLATFORM_WIN32
# include <Win32/geWin32Windows.h>
#endif

#include <geRTTIPlainField.h>
#include <geException.h>
#include <geMath.h>

#include <geCrashHandler.h>
#include <geDynLibManager.h>
#include <geFileSystem.h>
#include <geTime.h>
#include <geUnicode.h>

#include <SFML/Graphics.hpp>

#include <commdlg.h>
#include <imgui.h>
#include <imgui-sfml.h>

#include "RTSConfig.h"
#include "RTSApplication.h"
#include "RTSTiledMap.h"
#include "RTSPathMap.h"
#include "RTSGameMap.h"

#include "BFS.h"
#include "DFS.h"
#include "Best.h"

void
loadMapFromFile(RTSApplication* pApp);

void
mainMenu(RTSApplication* pApp);

void
PathFindingMenu(RTSApplication* pApp);

void
EditorMenu(RTSApplication* pApp);

RTSApplication::RTSApplication()
  : m_window(nullptr),
    m_fpsTimer(0.0f),
    m_fpsCounter(0.0f),
    m_framesPerSecond(0.0f)
{}

RTSApplication::~RTSApplication() {}

int32
RTSApplication::run() {
  CrashHandler::startUp();
  DynLibManager::startUp();
  Time::startUp();
  GameOptions::startUp();

  __try {
    initSystems();
    gameLoop();
    destroySystems();
  }
  __except (g_crashHandler().reportCrash(GetExceptionInformation())) {
    PlatformUtility::terminate(true);
  }

  GameOptions::shutDown();
  Time::shutDown();
  DynLibManager::shutDown();
  CrashHandler::shutDown();

  return 0;
}

void
RTSApplication::initSystems() {
  if (nullptr != m_window) {  //Window already initialized
    return; //Shouldn't do anything
  }

  //Create the application window
  m_window = ge_new<sf::RenderWindow>(sf::VideoMode(GameOptions::s_Resolution.x,
                                                    GameOptions::s_Resolution.y),
                                                    "RTS Game",
                                                    sf::Style::Fullscreen);
  if (nullptr == m_window) {
    GE_EXCEPT(InvalidStateException, "Couldn't create Application Window");
  }

  m_arialFont = ge_new<sf::Font>();
  if (nullptr == m_arialFont) {
    GE_EXCEPT(InvalidStateException, "Couldn't create a Font");
  }
  
  
  
  if (!m_arialFont->loadFromFile("Fonts/arial.ttf")) {
    GE_EXCEPT(FileNotFoundException, "Arial font not found");
  }/**/

  //m_window->setVerticalSyncEnabled(true);

  initGUI();


}

void
RTSApplication::initGUI() {
  ImGui::SFML::Init(*m_window);
}

void
RTSApplication::destroySystems() {
  ImGui::SFML::Shutdown();

  if (nullptr != m_window) {
    m_window->close();
    ge_delete(m_window);
  }

  if (nullptr != m_arialFont) {
    ge_delete(m_arialFont);
  }
}

void
RTSApplication::gameLoop() {
  if (nullptr == m_window) {  //Windows not yet initialized
    return; //Shouldn't do anything
  }

  postInit();

  while (m_window->isOpen()) {
    sf::Event event;
    while (m_window->pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      
      if (event.type == sf::Event::Closed) {
        m_window->close();
      }
    }

    g_time()._update();
    ge_frame_mark();
    updateFrame();
    renderFrame();
    ge_frame_clear();
  }

  postDestroy();
}

void
RTSApplication::updateFrame() {
  float deltaTime = g_time().getFrameDelta();
  
  m_fpsTimer += deltaTime;
  if (1.0f < m_fpsTimer) {
    m_framesPerSecond = m_fpsCounter;
    m_fpsCounter = 0.0f;
    m_fpsTimer = 0.0f;
  }
  m_fpsCounter += 1.0f;

  //Update the interface
  sf::Clock myClock;
  ImGui::SFML::Update(*m_window, myClock.getElapsedTime());

  //Begin the menu 
  mainMenu(this);

  PathFindingMenu(this);

  EditorMenu(this);

  //Check for camera movement
  Vector2 axisMovement(FORCE_INIT::kForceInitToZero);
  Vector2I mousePosition;
  mousePosition.x = sf::Mouse::getPosition(*m_window).x;
  mousePosition.y = sf::Mouse::getPosition(*m_window).y;

  if (0 == mousePosition.x ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
#ifdef MAP_IS_ISOMETRIC
    axisMovement += Vector2(-1.f, 1.f);
#else
    axisMovement += Vector2(-1.f, 0.f);
#endif
  }
  if (GameOptions::s_Resolution.x -1 == mousePosition.x ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
#ifdef MAP_IS_ISOMETRIC
    axisMovement += Vector2(1.f, -1.f);
#else
    axisMovement += Vector2(1.f, 0.f);
#endif
  }
  if (0 == mousePosition.y ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
#ifdef MAP_IS_ISOMETRIC
    axisMovement += Vector2(-1.f, -1.f);
#else
    axisMovement += Vector2(0.f, -1.f);
#endif
  }
  if (GameOptions::s_Resolution.y - 1 == mousePosition.y ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
#ifdef MAP_IS_ISOMETRIC
    axisMovement += Vector2(1.f, 1.f);
#else
    axisMovement += Vector2(0.f, 1.f);
#endif
  }

  axisMovement *= GameOptions::s_MapMovementSpeed * deltaTime;

  m_gameWorld.getGameMap()->moveCamera(axisMovement.x, axisMovement.y);
  m_gameWorld.getPathTiledMap()->moveCamera(axisMovement.x, axisMovement.y);

  
  //mousePosition
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    Vector2I tile;
    Vector2I tilePos;
    m_gameWorld.getGameMap()->getScreenToMapCoords(mousePosition.x, mousePosition.y, tile.x, tile.y);
    m_gameWorld.getGameMap()->getMapToScreenCoords(tile.x, tile.y, tilePos.x, tilePos.y);
     
    std::cout << "mousePos -> x = " << mousePosition.x << ", y = " << mousePosition.y << std::endl;
    std::cout << "Tile     -> x = " << tile.x << ", y = " << tile.y << std::endl;
    std::cout << "TilePos  -> x = " << tilePos.x << ", y = " << tilePos.y << std::endl;

    m_gameWorld.getGameMap()->setType(tile.x, tile.y, m_UI.tileTypeID);
    unitTest.SetPosition(static_cast<Vector2>(tilePos));

  }

  //m_gameWorld.getGameMap()->setColor(1,1, 252, 3, 3);

  //Update the world
  m_gameWorld.update(deltaTime);

  unitTest.Update(deltaTime);


  // PathFinder
  pFinder->update(deltaTime);


}

void
RTSApplication::renderFrame() {
  m_window->clear(sf::Color::Blue);

  // Render World
  m_gameWorld.render();
  //characterTest.draw();
  unitTest.Render();

  //Render Path Finder

  ImGui::SFML::Render(*m_window);

  pFinder->render(m_window);



  /*
  sf::Text text;
  text.setPosition(0.f, 30.f);
  text.setFont(*m_arialFont);
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::Red);
  text.setString( toString(1.0f/g_time().getFrameDelta()).c_str() );
  m_window->draw(text);
  /**/
  m_window->display();
}

void
RTSApplication::postInit() {
  m_gameWorld.init(m_window);
  m_gameWorld.updateResolutionData();

  // PathFinder
  m_start = { 0, 0 };
  m_goal = { 10, 10 };

  m_bfs.Init(&m_gameWorld, m_start, m_goal);
  m_dfs.Init(&m_gameWorld, m_start, m_goal);
  m_best.Init(&m_gameWorld, m_start, m_goal);
  m_dijkstra.Init(&m_gameWorld, m_start, m_goal);
  m_aStar.Init(&m_gameWorld, m_start, m_goal);

  m_UI.start[0] = m_start.x;
  m_UI.start[1] = m_start.y;

  m_UI.goal[0] = m_goal.x;
  m_UI.goal[1] = m_goal.y;  

  pFinder = &m_bfs;

  grassTexture.loadFromFile("C:/Users/angel/Documents/School/Github/Qurino_AI2/Bin/Textures/Terrain/terrain_Grass.png");
  characterTest.loadFromFile(m_window, "C:/Users/angel/Downloads/Test.png");
  unitTest.Init(m_window);


}

void
RTSApplication::postDestroy() {
  m_gameWorld.destroy();
}



void
loadMapFromFile(RTSApplication* pApp) {
  OPENFILENAMEW ofn = { 0 };

  WString fileName;
  fileName.resize(MAX_PATH);
  bool bMustLoad = false;

  Path currentDirectory = FileSystem::getWorkingDirectoryPath();

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = nullptr;
  ofn.lpstrDefExt = L".bmp";
  ofn.lpstrFilter = L"Bitmap File\0*.BMP\0All\0*.*\0";
  ofn.lpstrInitialDir = L"Maps\\";
  ofn.lpstrFile = &fileName[0];
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileNameW(&ofn)) {
    if (fileName.size() > 0) {
      bMustLoad = true;
    }
  }

  SetCurrentDirectoryW(UTF8::toWide(currentDirectory.toString()).c_str());

  if (bMustLoad) {
    pApp->getWorld()->getGameMap()->loadFromImageFile(pApp->getRenderWindow(),
                                                       toString(fileName));
  }
}

void
mainMenu(RTSApplication* pApp) {
  if (ImGui::BeginMainMenuBar()) 
  {
    if (ImGui::BeginMenu("Map")) 
    {
      if (ImGui::MenuItem("Load...", "CTRL+O")) {
        loadMapFromFile(pApp);
      }
      if (ImGui::MenuItem("Save...", "CTRL+S")) {

      }
      ImGui::Separator();

      if (ImGui::MenuItem("Quit", "CTRL+Q")) {
        pApp->getRenderWindow()->close();
      }

      ImGui::EndMenu();
    }
    
    ImGui::EndMainMenuBar();
  }

  ImGui::Begin("Game Options");
  {
    ImGui::Text("Framerate: %f", pApp->getFPS());

    ImGui::SliderFloat("Map movement speed X",
      &GameOptions::s_MapMovementSpeed.x,
      0.0f,
      10240.0f);
    ImGui::SliderFloat("Map movement speed Y",
      &GameOptions::s_MapMovementSpeed.y,
      0.0f,
      10240.0f);

    ImGui::Checkbox("Show grid", &GameOptions::s_MapShowGrid);
    ImGui::Checkbox("Show path", &GameOptions::s_renderPathTiles);
  }
  ImGui::End();

}



void
PathFindingMenu(RTSApplication* pApp)
{
  ImGui::Begin("PathFindingMenu");
  {
    Vector2I cMapSize = pApp->getWorld()->getGameMap()->getMapSize();
    string displayMapSize = "Game map Size x = " + to_string(cMapSize.x) + ", y = " + to_string(cMapSize.y);
    ImGui::Text(displayMapSize.c_str());
    //ImGui::Text("Selelct Path Type");


    UserInterface* UI = &pApp->m_UI;
    ImGui::Text("Selelct Path Type");

    if (ImGui::BeginCombo("SEARCH TYPE", UI->currentSearchType.c_str() ))
    {
      for (int i = 0; i < 5; i++)
      {
        bool is_selected = (UI->currentSearchType == UI->searchType[i]);
        if (ImGui::Selectable(UI->searchType[i].c_str(), is_selected))
        {
          UI->currentSearchType = UI->searchType[i];

        }
        if (is_selected)
        {
           ImGui::SetItemDefaultFocus();
        }
      }
      
      if (UI->currentSearchType == UI->searchType[0])
      {
        pApp->setPathFinder(pApp->m_bfs);
      }
      else if (UI->currentSearchType == UI->searchType[1])
      {
        pApp->setPathFinder(pApp->m_dfs);
      }
      else if (UI->currentSearchType == UI->searchType[2])
      {
        pApp->setPathFinder(pApp->m_best);
      }
      else if (UI->currentSearchType == UI->searchType[3])
      {
        pApp->setPathFinder(pApp->m_dijkstra);
      }
      else if (UI->currentSearchType == UI->searchType[4])
      {
        pApp->setPathFinder(pApp->m_aStar);
      }

      ImGui::EndCombo();
    }

    // ----------------- //
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    if (ImGui::Button("Start Search"))
    {
      pApp->getPathFinder()->startSearching();
    }
    if (ImGui::Button("Stop Search"))
    {
      pApp->getPathFinder()->stopSearching();
    }
    if (ImGui::Button("Reset Search"))
    {
      pApp->getPathFinder()->stopSearching();
      pApp->getPathFinder()->resetPath();
    }

    if(ImGui::InputInt2("Start position", UI->start))
    {
      pApp->pFinder->setStartCoord({ UI->start[0], UI->start[1] });
    }
    if (ImGui::InputInt2("Goal position", UI->goal))
    {
      pApp->pFinder->setTargetCoord({ UI->goal[0], UI->goal[1] });

    }

    // ----------------- //
    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
  }
  ImGui::End();
}

void
EditorMenu(RTSApplication* pApp)
{
  ImGui::Begin("EditorMenu");
  {
    UserInterface* UI = &pApp->m_UI;
    ImGui::Text("Selelct Path Type");



    if (ImGui::BeginCombo("SEARCH TYPE", UI->currentTileType.c_str(), ImGuiComboFlags_::ImGuiComboFlags_PopupAlignLeft))
    {
      for (int i = 0; i < 4; i++)
      {
        ImGui::Image(pApp->grassTexture, ImVec2(10, 10));
        ImGui::SameLine();
        bool is_selected = (UI->currentTileType == UI->tileType[i]);
        if (ImGui::Selectable(UI->tileType[i].c_str(), is_selected,ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick))
        {

          UI->currentTileType = UI->tileType[i];

        }

        if (is_selected)
        {
          //ImGui::SetItemDefaultFocus();
        }
      }

      if (UI->currentTileType == UI->tileType[0])
      {
        UI->tileTypeID = 0;
      }
      else if (UI->currentTileType == UI->tileType[1])
      {
        UI->tileTypeID = 1;
      }
      else if (UI->currentTileType == UI->tileType[2])
      {
        UI->tileTypeID = 2;
      }
      else if (UI->currentTileType == UI->tileType[3])
      {
        UI->tileTypeID = 3;
      }

      ImGui::EndCombo();
    }

    ImGui::Text("Current shit = ");
    ImGui::Image(pApp->grassTexture, ImVec2(75, 75));
  }
  ImGui::End();
}
