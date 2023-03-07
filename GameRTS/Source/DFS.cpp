#include "DFS.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"
#include "RTSGameMap.h"




void DFS::update(float deltaTime)
{
  DFS::PathFinder::update(deltaTime);

}

uint32 DFS::nextNodeID()
{
  return (openNodes.size() - 1);
}

void DFS::addConnections(Node* node)
{
  Vector2I nodeAux = { 0 , 0 };
  //consigue los siguientes nodos, en caso de no encotrar el target
  for (int i = 0; i < node->conections.nextNodes.size(); i++)
  {
    nodeAux = node->coord + node->conections.nextNodes[i];

    if (nodeAux.x >= 0 && nodeAux.y >= 0 &&
      nodeAux.x < world->getGameMap()->getMapSize().x && nodeAux.y < world->getGameMap()->getMapSize().y
      && world->getGameMap()->getType(nodeAux.x, nodeAux.y) == TERRAIN_TYPE::kGrass)
    {
      // agregamos el nodo a la lista abaierta si es que no esta
      if (nullptr == getNodeInOpenList(nodeAux) && nullptr == getNodeInClosedList(nodeAux))
      {
        openNodes.push_back(new Node(nodeAux, node));
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

  uint32 nodeID = nextNodeID();
  closedNodes.push_back(openNodes[nodeID]); // agregamos a los nodos cerrados 
  openNodes.erase(openNodes.begin() + int(nodeID));// Remove the first element

  //openNodes.pop_back(); // Remove the last element
  //openNodes.erase(openNodes.begin());// Remove the first element


  if (closedNodes[closedNodes.size() - 1]->coord == *targetCoord)
  {
    return SEARCHING_STATE::FOUND;
  }
  else
  {
    addConnections(closedNodes[closedNodes.size() - 1]);
    return SEARCHING_STATE::SEARCHING;
  }
}
