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

  if (searchingState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchingState = step();
  }
}

void DFS::render()
{
  // render
  if (searchingState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
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

void DFS::run()
{
  if (false == isNodesSeted)
  {
    setNodes();
  }
  //openNodes.push_back(new Node(startCoord, nullptr));

  if (step() != SEARCHING_STATE::FOUND)
  {
    showPath(targetCoord);
  }/**/
}

uint32 DFS::nextNodeID()
{
  return (openNodes.size() - 1);
}

void DFS::addConnections(const Node& node)
{
  Vector2I nodeAux = { 0 , 0 };
  //consigue los siguientes nodos, en caso de no encotrar el target
  for (int i = 0; i < node.conections.nextNodes.size(); i++)
  {
    nodeAux = closedNodes[closedNodes.size() - 1]->coord + node.conections.nextNodes[i];

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
}

// checa el nodo siguiente de la lista abierta (ultimo)
SEARCHING_STATE::E DFS::step()
{
  // open nodes list is empty
  if (openNodes.empty())
  {
    return SEARCHING_STATE::NOT_FOUND;
  }

  float nodeID = nextNodeID();
  closedNodes.push_back(openNodes[nodeID]); // agregamos a los nodos cerrados 
  openNodes.erase(openNodes.begin());// Remove the first element


  if (openNodes[nodeID]->coord == targetCoord)
  {
    return SEARCHING_STATE::FOUND;
  }
  else
  {
    addConnections(*closedNodes[closedNodes.size() - 1]);
    return SEARCHING_STATE::SEARCHING;
  }

  /*
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
  /**/
}
