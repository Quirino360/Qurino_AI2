#pragma once
#include "PathFinder.h"

class RTSTiledMap;

class BFS : public PathFinder
{
public:
  BFS() = default;

  ~BFS() = default;

  virtual void 
  Init(RTSTiledMap* _tileMap);

  virtual void
  update(float deltaTime);

  virtual void
  render();

  // main algorithm
  virtual void
  run();

  // search for new nodes and 
  virtual bool
  step();

private:

  Conections conections;

  float stepPerFrames = 1;

  float elapsedFrames = 0;

};

