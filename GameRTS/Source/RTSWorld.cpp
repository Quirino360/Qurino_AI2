#include "RTSWorld.h"
#include "RTSTiledMap.h"

#include "RTSUnitType.h"
//#include "RTSConfig.h"


RTSWorld::RTSWorld() {
  m_pTiledMap = nullptr;
  m_pTiledPathMap = nullptr;
  m_activeWalkerIndex = -1;	//-1 = Invalid index
}

RTSWorld::~RTSWorld() {
  destroy();
}

bool
RTSWorld::init(sf::RenderTarget* pTarget) {
  GE_ASSERT(nullptr == m_pTiledMap && "World was already initialized");
  destroy();

  GE_ASSERT(nullptr == m_pTiledPathMap && "Path Tiles was already initialized");
  destroy();

  m_pTarget = pTarget;

  //Initialize the map (right now it's an empty map)
  m_pTiledMap = ge_new<RTSTiledMap>();
  GE_ASSERT(m_pTiledMap);
  m_pTiledMap->init(m_pTarget, Vector2I(256, 256));

  m_pTiledPathMap  = ge_new<RTSTiledMap>();
  GE_ASSERT(m_pTiledPathMap);
  m_pTiledPathMap->init(m_pTarget, Vector2I(256, 256), "Textures/PathType", 
  {"Untiled", "Start", "Target", "OpenList", "ClosedList"});

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
  m_pTiledPathMap->setMapType(0);

  RTSGame::RTSUnitType unitTypes;
  unitTypes.loadAnimationData(m_pTarget, 1);

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

  if (nullptr != m_pTiledPathMap) {
    ge_delete(m_pTiledPathMap);
    m_pTiledPathMap = nullptr;
  }
}

void
RTSWorld::update(float deltaTime) {
  m_pTiledMap->update(deltaTime);
  m_pTiledPathMap->update(deltaTime);
}

void
RTSWorld::render() {
  m_pTiledMap->render();
  if (GameOptions::s_renderPathTiles == true)
  {
    m_pTiledPathMap->render();
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

  if (nullptr != m_pTiledPathMap) {
    Vector2I appResolution = g_gameOptions().s_Resolution;

    m_pTiledPathMap->setStart(0, 0);
    m_pTiledPathMap->setEnd(appResolution.x, appResolution.y - 175);

    //This ensures a clamp if necessary
    m_pTiledPathMap->moveCamera(0, 0);
  }
}

void
RTSWorld::setCurrentWalker(const int8 index) {
  //Revisamos que el walker exista (en modo de debug)
  GE_ASSERT(m_walkersList.size() > static_cast<SIZE_T>(index));

  m_activeWalker = m_walkersList[index];
  m_activeWalkerIndex = index;
}
