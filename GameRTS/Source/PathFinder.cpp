#include "PathFinder.h"

#include "RTSTiledMap.h"


PathFinder::PathFinder()
{

}

void PathFinder::update(float deltaTime)
{
}

void PathFinder::setNodes(const Vector2I& _startCoord, const Vector2I& _targetCoord)
{
  startCoord = _startCoord;
  targetCoord = _targetCoord;

  //nodes are valid coords
  if (startCoord.x == NULL && startCoord.y == NULL &&
    targetCoord.x == NULL && targetCoord.y == NULL)
  {
    return;
  }

  openNodes.push_back(new Node(startCoord, nullptr));

  isNodesSeted = true;
}

void PathFinder::showPath(Vector2I _target)
{
  Node trgt = Node(Vector2I(0,0), nullptr);
  for (int i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i]->coord == _target)
    {
      trgt = *closedNodes[i];
    }
  }
  
  Node* fatherNodeAux = trgt.fatherNode;

  while (fatherNodeAux != nullptr)
  {
    tileMap->setType(fatherNodeAux->coord.x, fatherNodeAux->coord.y, 1);
    fatherNodeAux = fatherNodeAux->fatherNode;
  }

}

bool PathFinder::isInOpenList(Vector2I _target)
{
  for (int i = 0; i < openNodes.size(); i++)
  {
     if (openNodes[i]->coord == _target)
     {
        return true;
     }
  }
  return false;
}

bool PathFinder::isInClosedList(Vector2I _target)
{
  for (int i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i]->coord == _target)
    {
      return true;
    }
  }
  return false;
}

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

// explorar los nodos
// checar que sea el nodo que buscamos
// guardar los nodos 
// escoger el siguiente nodo (esto es lo que cambia)


// *Crear una lista por los cuales fui avanzando?*