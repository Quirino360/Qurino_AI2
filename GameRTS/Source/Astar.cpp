#include "Astar.h"

void Astar::Init(RTSWorld* _world)
{
  world = _world;

  setNodes();
}

void Astar::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 1;

  if (searchState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchState = step();
  }
}

void Astar::render()
{
}

void Astar::run()
{
}

uint32 Astar::nextNodeID()
{
  return uint32();
}

void Astar::addConnections(Node* node)
{
}

SEARCHING_STATE::E Astar::step()
{
  return SEARCHING_STATE::E();
}
