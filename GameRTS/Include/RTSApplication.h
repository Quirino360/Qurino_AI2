#pragma once

#include "RTSTexture.h"
#include "RTSWorld.h"

#include "PathFinder.h";
#include "BFS.h"
#include "DFS.h"

using namespace geEngineSDK;

namespace sf{
  class RenderWindow;
  class Font;
}

class RTSApplication
{
public:
 RTSApplication();
 virtual ~RTSApplication();

 int32
 run();

 sf::RenderWindow*
 getRenderWindow() {
   return m_window;
 }

 RTSWorld*
 getWorld() {
   return &m_gameWorld;
 }

 float getFPS() const {
   return m_framesPerSecond;
 }

 PathFinder*
   getPathFinder() {
   return pFinder;
 }

 void
   setPathFinder(PathFinder* _pFinderType) {
   pFinder = _pFinderType;
 }

protected:

private:
  void
  initSystems();

  void
  initGUI();

  void
  destroySystems();

  void
  gameLoop();

  void
  postInit();

  void
  postDestroy();

  void
  updateFrame();
  
  void
  renderFrame();

public:
  BFS m_bfs;



private:
  sf::RenderWindow* m_window;
  sf::Font* m_arialFont;
  RTSWorld m_gameWorld;
  PathFinder* pFinder;
  //DFS m_dfs;




  float m_fpsTimer;
  float m_fpsCounter;
  float m_framesPerSecond;



};

// App -> World -> Map -> Tiles