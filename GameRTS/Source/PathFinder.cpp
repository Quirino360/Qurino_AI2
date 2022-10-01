#include "PathFinder.h"

#include "RTSTiledMap.h"


PathFinder::PathFinder()
{

}

void PathFinder::update()
{

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
    tileMap->setType(fatherNodeAux->coord.x, fatherNodeAux->coord.y, 3);
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