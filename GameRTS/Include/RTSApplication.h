#pragma once

#include "RTSTexture.h"
#include "RTSWorld.h"

#include "PathFinder.h";
#include "BFS.h"
#include "DFS.h"
#include "Best.h"
#include "Dijkstra.h"
#include "Astar.h"


//C:\Users\angel\Documents\School\Github\Qurino_AI2\Bin\RTS\assets\game_objects\units

using namespace geEngineSDK;

namespace sf{
  class RenderWindow;
  class Font;
}


struct UserInterface
{
  int start[2] = { 0 ,0 };
  int goal[2] = { 10 ,10 };
  string currentSearchType = "BREADTH FIRST SEARCH";
  string searchType[5] = { "BREADTH FIRST SEARCH",  "DEPTH FIRST SEARCH" , "BEST", "DIJKSTRA", "ASTAR" };

  string currentTileType = "WATER";
  string tileType[4] = { "WATER", "GRASS", "MARSH", "OBSTACLE" };
  int tileTypeID = 0;

};


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
   setPathFinder(PathFinder& _pFinderType) {
   pFinder = &_pFinderType;
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
  PathFinder* pFinder;
  BFS m_bfs;
  DFS m_dfs;
  Best m_best;
  Dijkstra m_dijkstra;
  Astar m_aStar;

  Vector2I m_start = { 0, 0 };
  Vector2I m_goal = { 10, 10 };
  
  UserInterface m_UI;

  sf::Texture a;
private:
  sf::RenderWindow* m_window;
  sf::Font* m_arialFont;
  RTSWorld m_gameWorld;

  float m_fpsTimer;
  float m_fpsCounter;
  float m_framesPerSecond;


};

// App -> World -> Map -> Tiles