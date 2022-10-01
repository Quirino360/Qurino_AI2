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
  update();

  // main algorithm
  virtual void
  run(const Vector2I& _startCoord, const Vector2I& _targetCoord);

  // search for new nodes and 
  virtual bool
  step();

private:

  Conections conections;

  bool canRun = true;
};

