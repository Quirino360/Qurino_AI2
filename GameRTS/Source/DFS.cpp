#include "DFS.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"


void DFS::Init(RTSWorld* _world)
{
  world = _world;

  setNodes();
}

void DFS::update(float deltaTime)
{
  elapsedFrames += 60;

  if (true == isFindingTarget)
  {
    if (elapsedFrames >= stepPerFrames)
    {
      run();
    }
  }
}

void DFS::render()
{
  // render
  if (true == isFindingTarget)
  {
    if (elapsedFrames >= stepPerFrames)
    {
      for (uint16 i = 0; i < openNodes.size(); i++)
      {
        world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 3);
      }

      for (uint16 i = 0; i < closedNodes.size(); i++)
      {
        world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 4);
      }

      world->getPathTiledMap()->setType(startCoord.x, startCoord.y, 1);
      world->getPathTiledMap()->setType(targetCoord.x, targetCoord.y, 2);
      elapsedFrames = 0;
    }
  }
}

void DFS::run()
{
  if (false == isNodesSeted)
  {
    setNodes();
  }

  //openNodes.push_back(new Node(startCoord, nullptr));

  Vector2I nodeAux = { 0 , 0 };


  if (isFindingTarget == true)
  {
    step();

    //consigue los siguientes nodos, en caso de no encotrar el target
    for (int i = 0; i < conections.nextNodes.size(); i++)
    {
      nodeAux = closedNodes[closedNodes.size() - 1]->coord + conections.nextNodes[i];

      if (nodeAux.x >= 0 && nodeAux.y >= 0 &&
        nodeAux.x < world->getTiledMap()->getMapSize().x && nodeAux.y < world->getTiledMap()->getMapSize().y
        && world->getTiledMap()->getType(nodeAux.x, nodeAux.y) != 3)
      {
        // agregamos el nodo a la lista abaierta si es que no esta
        if (false == isInOpenList(nodeAux) && false == isInClosedList(nodeAux))
        {
          openNodes.push_back(new Node(nodeAux, closedNodes[closedNodes.size() - 1], 0));
        }
      }
    }

    if (isFindingTarget == false)
    {
      for (uint16 i = 0; i < openNodes.size(); i++)
      {
        world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 3);
      }
      for (uint16 i = 0; i < closedNodes.size(); i++)
      {
        world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 4);
      }

      world->getPathTiledMap()->setType(startCoord.x, startCoord.y, 1);
      world->getPathTiledMap()->setType(targetCoord.x, targetCoord.y, 2);

      showPath(targetCoord);
    }
  }/**/
}

// checa el nodo siguiente de la lista abierta (ultimo)
bool DFS::step()
{

  //el nodo esta vacio
  if (openNodes.empty())
  {
    isFindingTarget = false; // ya no hay nada que buscar
    return true;
  }
  // econtro el nodo
  else if (openNodes[openNodes.size() -1]->coord == targetCoord)
  {
    isFindingTarget = false;

    closedNodes.push_back(openNodes[openNodes.size() - 1]); // agregamos a los nodos cerrados 
    openNodes.erase(openNodes.begin());// Remove the first element

    return true;
  }
  // no es el nodo que buscamos
  else
  {

    closedNodes.push_back(openNodes[openNodes.size() - 1]); // agregamos a los nodos cerrados 
    world->getPathTiledMap()->setType(openNodes[openNodes.size() - 1]->coord.x, openNodes[openNodes.size() - 1]->coord.y, 0);


    openNodes.pop_back();// Remove the last element

    return false;
  }
}
