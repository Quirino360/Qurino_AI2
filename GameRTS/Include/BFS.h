#pragma once
#include "PathFinder.h"

class RTSTiledMap;
class RTSWorld;

class BFS : public PathFinder
{
public:
  BFS() = default;

  ~BFS() = default;

  virtual void 
  Init(RTSWorld* _world);

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
};

