#pragma once

#include <gePrerequisitesUtil.h>
#include <geVector2I.h>

#include <SFML/Graphics.hpp>

//#include "PathFinder.h"

using namespace geEngineSDK;

class RTSGameMap;
class RTSPathMap;

class RTSMapGridWalker;

class RTSWorld
{
 public:
  RTSWorld();
  ~RTSWorld();

 public:
  bool
  init(sf::RenderTarget* pTarget);

  void
  destroy();

  void
  update(float deltaTime);

  void
  render();

  RTSGameMap*
  getGameMap() {
    return m_pTiledMap;
  }

  RTSPathMap*
  getPathTiledMap() {
    return m_pathMap;
  }

  void
  updateResolutionData();

  void
  setCurrentWalker(const int8 index);

 private:
   RTSGameMap* m_pTiledMap; // The map
   RTSPathMap* m_pathMap; // The map

  //List<RTSUnitType*> m_lstUnitTypes;
  //List<RTSUnit*> m_lstUnits;
  
  //Vector<RTSMapGridWalker*> m_walkersList;
  Vector<void*> m_walkersList;
  //RTSMapGridWalker* m_activeWalker;
  void* m_activeWalker;
  int8 m_activeWalkerIndex;

  sf::RenderTarget* m_pTarget;

};
