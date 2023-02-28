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

  m_gameWorld.getTiledMap()->moveCamera(axisMovement.x, axisMovement.y);
  m_gameWorld.getPathTiledMap()->moveCamera(axisMovement.x, axisMovement.y);

  
  //mousePosition
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    int xx, yy = 0;
    int tileX, tileY = 0;
    m_gameWorld.getTiledMap()->getMapToScreenCoords(0, 0, xx, yy);
    m_gameWorld.getTiledMap()->getScreenToMapCoords(mousePosition.x, mousePosition.y, tileX, tileY);
     
    std::cout << "mousePos -> x = " << mousePosition.x << ", y = " << mousePosition.y << std::endl;
    std::cout << "TilePos  -> x = " << xx << ", y = " << yy << std::endl;
    std::cout << "Tile     -> x = " << tileX << ", y = " << tileY << std::endl;

    m_gameWorld.getTiledMap()->setType(tileX, tileY, m_UI.tileType);

  }

  //m_gameWorld.getTiledMap()->setColor(1,1, 252, 3, 3);

  //Update the world
  m_gameWorld.update(deltaTime);

  // PathFinder
  pFinder->update(deltaTime);
}

void
RTSApplication::renderFrame() {
  m_window->clear(sf::Color::Blue);

  // Render World
  m_gameWorld.render();

  //Render Path Finder
  pFinder->render();

  ImGui::SFML::Render(*m_window);

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
    pApp->getWorld()->getTiledMap()->loadFromImageFile(pApp->getRenderWindow(),
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
    UserInterface* UI = &pApp->m_UI;
    ImGui::Text("Selelct Path Type");
    if (ImGui::SliderInt(UI->searchName.c_str(), &UI->searchType, 1, 5))
    {
      if (1 == UI->searchType)
      {
        UI->searchName = "BREADTH FIRST SEARCH";
        pApp->setPathFinder(pApp->m_bfs);
      }
      else if (2 == UI->searchType)
      {
        UI->searchName = "DEPTH FIRST SEARCH";
        pApp->setPathFinder(pApp->m_dfs);
      }
      else if (3 == UI->searchType)
      {
        UI->searchName = "BEST";
        pApp->setPathFinder(pApp->m_best);
      }
      else if (4 == UI->searchType)
      {
        UI->searchName = "DIJKSTRA";
        pApp->setPathFinder(pApp->m_dijkstra);
      }
      else if (5 == UI->searchType)
      {
        UI->searchName = "ASTAR";
        pApp->setPathFinder(pApp->m_aStar);
      }
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
    if (ImGui::SliderInt(UI->tileTypeName.c_str(), &UI->tileType, 0, 3))
    {
      if (0 == UI->tileType)
      {
        UI->tileTypeName = "WATER";
      }
      else if (1 == UI->tileType)
      {
        UI->tileTypeName = "GRASS";
      }
      else if (2 == UI->tileType)
      {
        UI->tileTypeName = "MARSH";
      }
      else if (3 == UI->tileType)
      {
        UI->tileTypeName = "OBSTACLE";
      }
    }
  }
  ImGui::End();
}
