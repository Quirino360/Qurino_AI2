#pragma once
#include "PathFinder.h"

class RTSTiledMap;

class DFS : public PathFinder
{
public:
  DFS() = default;

  ~DFS() = default;

  virtual void
  Init(RTSWorld* _world);

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

