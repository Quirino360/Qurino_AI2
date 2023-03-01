#include "Dijkstra.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"
#include "RTSGameMap.h"


void Dijkstra::update(float deltaTime)
{
  Dijkstra::PathFinder::update(deltaTime);
}

uint32 Dijkstra::nextNodeID()
{
  //*get next node* // ya no esta en lista abierta y lo pone en la mano
 // ver cual es la ruta mas barata
  float nextToCheck = 0;
  float cheapest = 999999999999;
  float totalWeight = 0;

  for (uint32 i = 0; i < openNodes.size(); i++)
  {
    totalWeight = openNodes[i]->weight;
    if (totalWeight < cheapest)
    {
      cheapest = totalWeight;
      nextToCheck = i;
    };
  }

  return nextToCheck;
}

void Dijkstra::addConnections(Node* node)
{
  //adicionar los nodos a la lista de posibilidades (funcion)
    //consigue los siguientes nodos, en caso de no encotrar el target
  Vector2I coordAux = { 0 , 0 };
  float weightAux = 0;

  for (int i = 0; i < node->conections.nextNodes.size(); i++)
  {
    // ultimo nodo en meterse a la lista cerrada
    coordAux = node->coord + node->conections.nextNodes[i];

    // si esta adentro del mapa y su tipo no es obstaculo
    if (coordAux.x >= 0 && coordAux.y >= 0 &&
      coordAux.x < world->getGameMap()->getMapSize().x && coordAux.y < world->getGameMap()->getMapSize().y
      && world->getGameMap()->getType(coordAux.x, coordAux.y) == 1)
    {
      
      weightAux = world->getGameMap()->getCost(coordAux.x, coordAux.y) + node->weight;

      Node* nodeInClosedList = getNodeInClosedList(coordAux);
      if (nullptr != nodeInClosedList)
      {
        // si tiene una ruta mas barata, se cambia el padre y el peso
        if (weightAux < nodeInClosedList->weight)
        {
          nodeInClosedList->fatherNode = node;
          nodeInClosedList->setWeight(weightAux);
        }
      }
      else if (nullptr == getNodeInOpenList(coordAux))
      {
        openNodes.push_back(new Node(coordAux, node, 0, weightAux));
      }
    }
  }

}

SEARCHING_STATE::E Dijkstra::step()
{
  // open nodes list is empty
  if (openNodes.empty())
  {
    return SEARCHING_STATE::NOT_FOUND;
  }

  int32 nodeID = nextNodeID();
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
}
 