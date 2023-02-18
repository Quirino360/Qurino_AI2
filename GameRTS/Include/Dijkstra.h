#pragma once
#include "PathFinder.h"


class Dijkstra : public PathFinder
{
public:
  Dijkstra() = default;

  ~Dijkstra() = default;

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

