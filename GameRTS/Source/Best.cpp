#include "Best.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"



void Best::update(float deltaTime)
{
  Best::PathFinder::update(deltaTime);
}

uint32 Best::nextNodeID()
{
  //*get next node* // ya no esta en lista abierta y lo pone en la mano
  // ver cual es la ruta mas barata
  float nextToCheck = 0;
  float shorter = 999999999999;
  float totalDistance = 0;

  for (uint32 i = 0; i < openNodes.size(); i++)
  {
    totalDistance = openNodes[i]->distance;
    if (totalDistance < shorter)
    {
      shorter = totalDistance;
      nextToCheck = i;
    };
  }

  return nextToCheck;
}


void Best::addConnections(Node* node)
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
      && world->getTiledMap()->getType(coordAux.x, coordAux.y) == 1)
    {
      float distanceAux = 0;

      //
      auto dist = *targetCoord - coordAux;
      distanceAux = dist.size();

      Node* nodeInClosedList = getNodeInClosedList(coordAux);
      if (nullptr != nodeInClosedList)
      {
        // si tiene una ruta mas corta, se cambia la ruta
        if (distanceAux < nodeInClosedList->distance)
        {
          nodeInClosedList->fatherNode = node;
          nodeInClosedList->setDistance(distanceAux);
        }
      }
      // agregamos el nodo a la lista abaierta, si es que no esta en la lista abierta o cerrada
      else if (nullptr == getNodeInOpenList(coordAux))
      {
        openNodes.push_back(new Node(coordAux, node, distanceAux));
      }
    }
  }
}

// checa el nodo siguiente de la lista abierta (primero)
SEARCHING_STATE::E Best::step()
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

//que tengo,limpio las listas, recibo un nodo inicial y el de buscar
// el nodo inicicial, meterla en la lista de posibilidades (setup)
// dar un paso, buscar en la lista, si esta vacia, ya valio, 