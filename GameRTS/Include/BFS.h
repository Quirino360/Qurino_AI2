#pragma once
#include "PathFinder.h"

class BFS : public PathFinder
{
public:
  BFS() = default;

  ~BFS() = default;

  virtual void
  update(float deltaTime);

  virtual uint32
  nextNodeID();

  virtual void
  addConnections(Node* node);

  virtual SEARCHING_STATE::E
  step();

private:
};

