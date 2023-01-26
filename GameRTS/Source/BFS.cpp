#include "BFS.h"
#include "RTSTiledMap.h"


void BFS::Init(RTSTiledMap* _tileMap)
{
  tileMap = _tileMap;
}

void BFS::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 60;

  if (true == isFindingTarget)
  {
    if (elapsedFrames >= stepPerFrames)
    {
      run();
    }
  }
}

void BFS::render()
{
  // render
  if (true == isFindingTarget)
  {
    if (elapsedFrames >= stepPerFrames)
    {
      for (uint16 i = 0; i < openNodes.size(); i++)
      {
        tileMap->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 0);
      }

      for (uint16 i = 0; i < closedNodes.size(); i++)
      {
        tileMap->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 2);
      }

      elapsedFrames = 0;

    }
  }
}

void BFS::run()
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
      isFindingTarget = false; // ya no hay nada que buscar
      return true;
    }
    // econtro el target node
    else if (openNodes[0]->coord == targetCoord)
    {
      isTargetFounded = true;
      isFindingTarget = false;

      closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
      openNodes.erase(openNodes.begin());// Remove the first element

      showPath(targetCoord);

      return true;
    }
    // no es el target node
    else
    {
      closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
      openNodes.erase(openNodes.begin());// Remove the first element

      return false;
    }
}

