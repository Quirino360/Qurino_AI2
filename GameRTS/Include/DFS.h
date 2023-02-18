#pragma once
#include "PathFinder.h"

class DFS : public PathFinder
{
public:
  DFS() = default;

  ~DFS() = default;

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

