#pragma once
#include "PathFinder.h"

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

  virtual uint32
  nextNodeID();

  virtual void
  addConnections(const Node& node);

  virtual SEARCHING_STATE::E
  step();

private:
};

