#pragma once
#include "PathFinder.h"
class Astar : public PathFinder
{
public:
  Astar() = default;

  ~Astar() = default;

  virtual void
  update(float deltaTime);

  virtual uint32
  nextNodeID();

  virtual void
  addConnections(Node* node);

  virtual SEARCHING_STATE::E
  step();
};

