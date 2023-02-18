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

void PathFinder::update(float deltaTime)
{


}

void PathFinder::setNodes(const Vector2I& _startCoord, const Vector2I& _targetCoord)
{
  //nodes are valid coords
  if (startCoord.x == NULL && startCoord.y == NULL &&
    targetCoord.x == NULL && targetCoord.y == NULL)
  {
    isNodesSeted = false;
    return;
  }

  if (true == isNodesSeted)
  {
    world->getPathTiledMap()->setType(startCoord.x, startCoord.y, 0);
    world->getPathTiledMap()->setType(targetCoord.x, targetCoord.y, 0);

  }

  startCoord = _startCoord;
  targetCoord = _targetCoord;

  openNodes.clear();
  
  auto dist = targetCoord - startCoord;
  openNodes.push_back(new Node(startCoord, nullptr, dist.size(), world->getTiledMap()->getCost(startCoord.x, startCoord.y)));

  world->getPathTiledMap()->setType(startCoord.x, startCoord.y, 1);
  world->getPathTiledMap()->setType(targetCoord.x, targetCoord.y, 2);

  isNodesSeted = true;
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
      delete openNodes[i];
    }
  }
  openNodes.clear();

  for (uint32 i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i] != nullptr)
    {
      delete closedNodes[i];
    }
  }
  closedNodes.clear();

  isNodesSeted = false;

}



// explorar los nodos
// checar que sea el nodo que buscamos
// guardar los nodos 
// escoger el siguiente nodo (esto es lo que cambia)


// *Crear una lista por los cuales fui avanzando?*