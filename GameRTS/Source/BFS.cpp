#include "BFS.h"
#include "RTSTiledMap.h"


void BFS::Init(RTSTiledMap* _tileMap)
{
  tileMap = _tileMap;
}

void BFS::update(float deltaTime)
{
  // show step by step
  if (true == isStepByStep && false == isTargetFounded)
  {
    elapsedTimePerStep += deltaTime;
    if (elapsedTimePerStep >= timePerStep)
    {
      runStepByStep(Vector2I(0, 0), Vector2I(10, 10));
      elapsedTimePerStep = 0;
    }
  }
  if (false == isStepByStep)
  {
    run(Vector2I(0,0), Vector2I(10, 10));
  }
}

void BFS::run(const Vector2I& _startCoord, const Vector2I& _targetCoord)
{
  startCoord = _startCoord;
  targetCoord = _targetCoord;

  if (startCoord.x == NULL && startCoord.y == NULL &&
    targetCoord.x == NULL && targetCoord.y == NULL)
  {
    return;
  }

  openNodes.push_back( new Node(startCoord, nullptr));

  Vector2I nodeAux = {0 , 0};
  
  // mientras no encuentre el target node
  while (step() == false)
  {
    //consigue los siguientes nodos, en caso de no encotrar el target
    for (int i = 0; i < conections.nextNodes.size(); i++)
    {
      // Los nodos siguiente 
      nodeAux = closedNodes[closedNodes.size() - 1]->coord + conections.nextNodes[i];

      if (nodeAux.x >= 0 && nodeAux.y >= 0 &&
      nodeAux.x < tileMap->getMapSize().x && nodeAux.y < tileMap->getMapSize().y
      && tileMap->getType(nodeAux.x, nodeAux.y) == 1)
      {
        // agregamos el nodo a la lista abaierta si es que no esta en ninguna de las 2 listas
        if (false == isInOpenList(nodeAux) && false == isInClosedList(nodeAux))
        {
          openNodes.push_back( new Node(nodeAux, closedNodes[closedNodes.size() - 1]));
        }
      }
    }
  }
}

void BFS::runStepByStep(const Vector2I& _startCoord, const Vector2I& _targetCoord)
{
  if (false == isNodesSeted)
  {
    setNodes();
  }

  Vector2I nodeAux = { 0 , 0 };

  // mientras no encuentre el target node
  if(isTargetFounded == false)
  {
    step();
    //consigue los siguientes nodos, en caso de no encotrar el target
    for (int i = 0; i < conections.nextNodes.size(); i++)
    {
      // Los nodos siguiente 
      nodeAux = closedNodes[closedNodes.size() - 1]->coord + conections.nextNodes[i];

      if (nodeAux.x >= 0 && nodeAux.y >= 0 &&
        nodeAux.x < tileMap->getMapSize().x && nodeAux.y < tileMap->getMapSize().y
        && tileMap->getType(nodeAux.x, nodeAux.y) == 1)
      {
        // agregamos el nodo a la lista abaierta si es que no esta en ninguna de las 2 listas
        if (false == isInOpenList(nodeAux) && false == isInClosedList(nodeAux))
        {
          openNodes.push_back(new Node(nodeAux, closedNodes[closedNodes.size() - 1]));
        }
      }
    }
  }
}

// checa el nodo siguiente de la lista abierta (primero)
bool BFS::step()
{
   // ya no hay nodos y no se encontro el target node
    if (openNodes.empty())
    {
      isTargetFounded = true; //no lo encontro pero se pone true
      return true;
    }
    // econtro el target node
    else if (openNodes[0]->coord == targetCoord)
    {
      isTargetFounded = true;

      closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
      openNodes.erase(openNodes.begin());// Remove the first element

      showPath(targetCoord);
      return true;
    }
    // no es el target node
    else
    {
      closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
      tileMap->setType(openNodes[0]->coord.x, openNodes[0]->coord.y, 0); 

      openNodes.erase(openNodes.begin());// Remove the first element

      return false;
    }
}

void BFS::setNodes(const Vector2I& _startCoord, const Vector2I& _targetCoord)
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
