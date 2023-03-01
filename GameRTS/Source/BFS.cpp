#include "BFS.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"
#include "RTSGameMap.h"



void BFS::update(float deltaTime)
{
  BFS::PathFinder::update(deltaTime);

}

uint32 BFS::nextNodeID()
{
  return 0;
}

void BFS::addConnections(Node* node)
{
  Vector2I nodeAux = { 0 , 0 };

  //consigue los siguientes nodos, en caso de no encotrar el target
  for (int i = 0; i < node->conections.nextNodes.size(); i++)
  {
    // Los nodos siguiente 
    nodeAux = node->coord + node->conections.nextNodes[i];

    if (nodeAux.x >= 0 && nodeAux.y >= 0 &&
      nodeAux.x < world->getGameMap()->getMapSize().x && nodeAux.y < world->getGameMap()->getMapSize().y
      && world->getGameMap()->getType(nodeAux.x, nodeAux.y) == 1) //cambiar a constantes
    {
      // agregamos el nodo a la lista abaierta si es que no esta en ninguna de las 2 listas
      if (nullptr == getNodeInOpenList(nodeAux) && nullptr == getNodeInClosedList(nodeAux))
      {
        openNodes.push_back(new Node(nodeAux, node, 0));
      }
    }
  }
}

// checa el nodo siguiente de la lista abierta (primero)
SEARCHING_STATE::E BFS::step()
{
  // open nodes list is empty
  if (openNodes.empty())
  {
    return SEARCHING_STATE::NOT_FOUND;
  }

  uint32 nodeID = nextNodeID();
  closedNodes.push_back(openNodes[nodeID]); // agregamos a los nodos cerrados 
  openNodes.erase(openNodes.begin() + int(nodeID));// Remove the first element


  if (closedNodes[closedNodes.size() - 1]->coord == *targetCoord)
  {
    return SEARCHING_STATE::FOUND;
  }
  else
  {
    addConnections(closedNodes[closedNodes.size() - 1]);
    return SEARCHING_STATE::SEARCHING;
  }

  /*
  // ya no hay nodos y no se encontro el target node
  if (openNodes.empty())
  {
    isFindingTarget = false; // ya no hay nada que buscar
    return true;
  }
  // econtro el target node
  else if (openNodes[0]->coord == targetCoord)
  {
    isFindingTarget = false;

    closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
    openNodes.erase(openNodes.begin());// Remove the first element

    return true;
  }
  // no es el target node
  else
  {
    closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
    openNodes.erase(openNodes.begin());// Remove the first element

    return false;
  }/**/
}

