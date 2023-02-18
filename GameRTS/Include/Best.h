#pragma once
#include "PathFinder.h"


class Best : public PathFinder
{
public:
  Best() = default;

  ~Best() = default;

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

