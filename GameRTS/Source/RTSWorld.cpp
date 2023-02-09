#include "RTSWorld.h"
#include "RTSTiledMap.h"

#include "RTSUnitType.h"
//#include "RTSConfig.h"


RTSWorld::RTSWorld() {
  m_pTiledMap = nullptr;
  m_pathMap = nullptr;
  m_activeWalkerIndex = -1; 	//-1 = Invalid index
}

RTSWorld::~RTSWorld() {
  destroy();
}

bool
RTSWorld::init(sf::RenderTarget* pTarget) {
  GE_ASSERT(nullptr == m_pTiledMap && "World was already initialized");
  destroy();

  GE_ASSERT(nullptr == m_pathMap && "Path Tiles was already initialized");
  destroy();

  m_pTarget = pTarget;

  //Initialize the map (right now it's an empty map)
  m_pTiledMap = ge_new<RTSTiledMap>();
  GE_ASSERT(m_pTiledMap);
  m_pTiledMap->init(m_pTarget, Vector2I(256, 256));

  m_pathMap  = ge_new<RTSTiledMap>();
  GE_ASSERT(m_pathMap);
  m_pathMap->init(m_pTarget, Vector2I(256, 256), "Textures/PathType", 
  {"Untiled", "Start", "Target", "OpenList", "ClosedList", "Path"});

  //Create the path finding classes and push them to the walker list
  //m_walkersList.push_back(ge_new<RTSBreadthFirstSearchMapGridWalker>(m_pTiledMap));

  //Init the walker objects
/*
  for (SIZE_T it = 0; it < m_walkersList.size(); ++it) {
    m_walkersList[it]->init();
  }

  //Set the first walker as the active walker
  setCurrentWalker(m_walkersList.size() > 0 ? 0 : -1);
*/

  m_pTiledMap->setMapType(1);
  m_pathMap->setMapType(0);

  /*uint32 randNum;
  randNum = 0;
  for (uint32 i = 0; i < m_pTiledMap->getMapSize().x; i++)
  {
    for (uint32 j = 0; j < m_pTiledMap->getMapSize().y; j++)
    {
      randNum = rand() % 10; // range 0 to 99
      m_pTiledMap->setCost(i, j, randNum);
    }
  }/**/

  for (uint32 i = 1; i < 9; i++)
  {
    for (uint32 j = 1; j < 9; j++)
    {
      getTiledMap()->setType(i, j, 0);
    }
  }

  for (uint32 i = 0; i < m_pTiledMap->getMapSize().x; i++)
  {
    for (uint32 j = 0; j < m_pTiledMap->getMapSize().y; j++)
    {
      if (m_pTiledMap->getType(i, j) == 0)
      {
        m_pTiledMap->setCost(i, j, 10);
      }
      if (m_pTiledMap->getType(i, j) == 1)
      {
        m_pTiledMap->setCost(i, j, 1);
      }
      if (m_pTiledMap->getType(i, j) == 2)
      {
        m_pTiledMap->setCost(i, j, 5);
      }
    }
  }

  //set cost by tile type

  RTSGame::RTSUnitType unitTypes;
  //unitTypes.loadAnimationData(m_pTarget, 1);

  return true;
}

void
RTSWorld::destroy() {
 //Destroy all the walkers
  while (m_walkersList.size() > 0) {
    ge_delete(m_walkersList.back());
    m_walkersList.pop_back();
  }

  //As the last step, destroy the full map
  if (nullptr != m_pTiledMap) {
    ge_delete(m_pTiledMap);
    m_pTiledMap = nullptr;
  }

  if (nullptr != m_pathMap) {
    ge_delete(m_pathMap);
    m_pathMap = nullptr;
  }
}

void
RTSWorld::update(float deltaTime) {
  m_pTiledMap->update(deltaTime);
  m_pathMap->update(deltaTime);
}

void
RTSWorld::render() {
  m_pTiledMap->render();
  if (GameOptions::s_renderPathTiles == true)
  {
    m_pathMap->render();
  }
}

void
RTSWorld::updateResolutionData() {
  if (nullptr != m_pTiledMap) {
    Vector2I appResolution = g_gameOptions().s_Resolution;
    
    m_pTiledMap->setStart(0, 0);
    m_pTiledMap->setEnd(appResolution.x, appResolution.y - 175);
    
    //This ensures a clamp if necessary
    m_pTiledMap->moveCamera(0, 0);
  }

  if (nullptr != m_pathMap) {
    Vector2I appResolution = g_gameOptions().s_Resolution;

    m_pathMap->setStart(0, 0);
    m_pathMap->setEnd(appResolution.x, appResolution.y - 175);

    //This ensures a clamp if necessary
    m_pathMap->moveCamera(0, 0);
  }
}

void
RTSWorld::setCurrentWalker(const int8 index) {
  //Revisamos que el walker exista (en modo de debug)
  GE_ASSERT(m_walkersList.size() > static_cast<SIZE_T>(index));

  m_activeWalker = m_walkersList[index];
  m_activeWalkerIndex = index;
}
