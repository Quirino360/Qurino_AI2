#pragma once
#include "PathFinder.h"


class Best : public PathFinder
{
public:
  Best() = default;

  ~Best() = default;

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
  addConnections(Node* node);

  virtual SEARCHING_STATE::E
  step();

private:
};

