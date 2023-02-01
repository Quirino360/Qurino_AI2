#include "Best.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"


void Best::Init(RTSWorld* _world)
{
  world = _world;
}

void Best::update()
{
  if (canRun)
  {
    run(Vector2I(0, 0), Vector2I(10, 10));
    canRun = false;
  }
}

void Best::run(const Vector2I& _startCoord, const Vector2I& _targetCoord)
{
  startCoord = _startCoord;
  targetCoord = _targetCoord;

  if (startCoord.x == NULL && startCoord.y == NULL &&
    targetCoord.x == NULL && targetCoord.y == NULL)
  {
    return;
  }

  openNodes.push_back(new Node(startCoord, nullptr));

  Vector2I coordAux = { 0 , 0 };

  bool isOnClosedList = false;

  while (step() == false)
  {
    //consigue los siguientes nodos, en caso de no encotrar el target
    for (int i = 0; i < conections.nextNodes.size(); i++)
    {
      coordAux = closedNodes[closedNodes.size() - 1]->coord + conections.nextNodes[i];

      if (coordAux.x >= 0 && coordAux.y >= 0 &&
        coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
        && world->getTiledMap()->getType(coordAux.x, coordAux.y) == 1)
      {
        // agregamos el nodo a la lista abaierta si es que no esta
        if (false == isInOpenList(coordAux) && false == isInClosedList(coordAux))
        {
          openNodes.push_back(new Node(coordAux, closedNodes[closedNodes.size() - 1]));
        }

        //cambiamos el nodo padre a la ruta menos pesada
        Node toBeNode = Node(coordAux, closedNodes[closedNodes.size() - 1]);
        if (isInClosedList(coordAux) == true && toBeNode.getWeight() < getNodeInClosedList(coordAux)->getWeight())
        {
          getNodeInClosedList(coordAux)->fatherNode = toBeNode.fatherNode;
        }
      }
    }
  }
}

// checa el nodo siguiente de la lista abierta (primero)
bool Best::step()
{

  //el nodo esta vacio
  if (openNodes.empty())
  {
    return true;
  }
  // econtro el nodo
  else if (openNodes[0]->coord == targetCoord)
  {
    closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
    openNodes.erase(openNodes.begin());// Remove the first element

    showPath(targetCoord);
    return true;
  }
  // no es el nodo que buscamos
  else
  {
    closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
    world->getPathTiledMap()->setType(openNodes[0]->coord.x, openNodes[0]->coord.y, 0);

    openNodes.erase(openNodes.begin());// Remove the first element

    return false;
  }
}
