#include "Astar.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"

void Astar::update(float deltaTime)
{
  Astar::PathFinder::update(deltaTime);
}

uint32 Astar::nextNodeID()
{
  //*get next node* // ya no esta en lista abierta y lo pone en la mano
// ver cual es la ruta mas barata
  float nextToCheck = 0;
  float bestScore = 999999999999;
  float totalScore = 0;

  for (uint32 i = 0; i < openNodes.size(); i++)
  {
    totalScore = openNodes[i]->distance + openNodes[i]->weight;
    if (totalScore < bestScore)
    {
      bestScore = totalScore;
      nextToCheck = i;
    };
  }

  return nextToCheck;
}

// si el algoritmo tiene euristica usar la euristica en otra funcion

void Astar::addConnections(Node* node)
{
  //adicionar los nodos a la lista de posibilidades (funcion)
   //consigue los siguientes nodos, en caso de no encotrar el target
  Vector2I coordAux = { 0 , 0 };


  for (int i = 0; i < node->conections.nextNodes.size(); i++)
  {
    // ultimo nodo en meterse a la lista cerrada
    coordAux = node->coord + node->conections.nextNodes[i];

    // si esta adentro del mapa y su tipo no es obstaculo
    if (coordAux.x >= 0 && coordAux.y >= 0 &&
      coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
      && world->getTiledMap()->getType(coordAux.x, coordAux.y) != 3)
    {
      //
      float distanceAux = 0;
      float weightAux = 0;

      auto dist = *targetCoord - coordAux;
      distanceAux = dist.size();
      weightAux = world->getTiledMap()->getCost(coordAux.x, coordAux.y) + node->weight;

      Node* nodeInClosedList = getNodeInClosedList(coordAux);
      if (nullptr != nodeInClosedList)
      {
        // si tiene una ruta mas corta, se cambia la ruta
        if (distanceAux + weightAux < nodeInClosedList->distance + weightAux)
        {
          nodeInClosedList->fatherNode = node;
          nodeInClosedList->setDistance(distanceAux);
          nodeInClosedList->setWeight(weightAux);
        }
      }
      // agregamos el nodo a la lista abaierta, si es que no esta en la lista abierta o cerrada
      else if (nullptr == getNodeInOpenList(coordAux))
      {
        openNodes.push_back(new Node(coordAux, node, distanceAux, weightAux));
      }
    }
  }
}

SEARCHING_STATE::E Astar::step()
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
}
