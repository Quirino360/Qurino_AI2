#include "PathFinder.h"

#include "RTSWorld.h"
#include "RTSTiledMap.h"

 const Vector2I Node::Conections::TOP = Vector2I(0, -1);
 const Vector2I Node::Conections::TOPRIGHT = Vector2I(1, -1);
 const Vector2I Node::Conections::TOPLEFT = Vector2I(-1, -1);
 const Vector2I Node::Conections::LEFT = Vector2I(-1, 0);
 const Vector2I Node::Conections::RIGHT = Vector2I(1, 0);
 const Vector2I Node::Conections::DOWN = Vector2I(0, 1);
 const Vector2I Node::Conections::DOWNRIGHT = Vector2I(1, 1);
 const Vector2I Node::Conections::DOWNLEFT = Vector2I(-1, 1);
 


PathFinder::PathFinder()
{

}

void PathFinder::Init(RTSWorld* _world, Vector2I& _startCoord, Vector2I& _targetCoord)
{
  world = _world;

  startCoord = &_startCoord;
  targetCoord = &_targetCoord;

  setStartCoord(*startCoord);
  setTargetCoord(*targetCoord);

}

void PathFinder::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 1;

  if (searchState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchState = step();
  }
}

void PathFinder::render()
{
  // render
  if (searchState != SEARCHING_STATE::E::FOUND && elapsedFrames >= stepPerFrames)
  {
    for (uint16 i = 0; i < openNodes.size(); i++)
    {
      world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 3);
    }

    for (uint16 i = 0; i < closedNodes.size(); i++)
    {
      world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 4);
    }
    world->getPathTiledMap()->setType(startCoord->x, startCoord->y, 1);
    world->getPathTiledMap()->setType(targetCoord->x, targetCoord->y, 2);

    elapsedFrames = 0;
  }
  else if (searchState == SEARCHING_STATE::E::FOUND && elapsedFrames >= stepPerFrames)
  {
    showPath(*targetCoord);
  }
}

void PathFinder::showPath(Vector2I _target)
{
  Node trgt = Node(Vector2I(0,0), nullptr, 0);
  for (int i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i]->coord == _target)
    {
      trgt = *closedNodes[i];
    }
  }

  world->getPathTiledMap()->setType(trgt.coord.x, trgt.coord.y, 5);
  
  Node* fatherNodeAux = trgt.fatherNode;

  while (fatherNodeAux != nullptr)
  {
    world->getPathTiledMap()->setType(fatherNodeAux->coord.x, fatherNodeAux->coord.y, 5);
    fatherNodeAux = fatherNodeAux->fatherNode;
  }

}

void PathFinder::setStartCoord(const Vector2I coord)
{
  world->getPathTiledMap()->setType(startCoord->x, startCoord->y, 0);
  *startCoord = coord;

  openNodes.clear();
  auto dist = *targetCoord - *startCoord;
  openNodes.push_back(new Node(*startCoord, nullptr, dist.size(), 0));
}

void PathFinder::setTargetCoord(const Vector2I coord)
{ 
  world->getPathTiledMap()->setType(targetCoord->x, targetCoord->y, 0);
  *targetCoord = coord;
  
  openNodes.clear();
  auto dist = *targetCoord - *startCoord;
  openNodes.push_back(new Node(*startCoord, nullptr, dist.size(), 0));
}

// como optimzar, para despues
Node* PathFinder::getNodeInClosedList(Vector2I _target)
{
  for (int i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i]->coord == _target)
    {
      return closedNodes[i];
    }
  }
  return nullptr;
}

Node* PathFinder::getNodeInOpenList(Vector2I _target)
{
  for (int i = 0; i < openNodes.size(); i++)
  {
    if (openNodes[i]->coord == _target)
    {
      return openNodes[i];
    }
  }
  return nullptr;
}

void PathFinder::resetPath()
{
  for (uint32 i = 0; i < openNodes.size(); i++)
  {
    if (openNodes[i] != nullptr)
    {
      world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 0);
      delete openNodes[i];
    }
  }
  openNodes.clear();

  for (uint32 i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i] != nullptr)
    {
      world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 0);
      delete closedNodes[i];
    }
  }
  closedNodes.clear();

  searchState = SEARCHING_STATE::NOT_SEARCHING;

  setStartCoord(*startCoord);
  setTargetCoord(*targetCoord);
}



// explorar los nodos
// checar que sea el nodo que buscamos
// guardar los nodos 
// escoger el siguiente nodo (esto es lo que cambia)


// *Crear una lista por los cuales fui avanzando?*