#include "Dijkstra.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"

void Dijkstra::Init(RTSWorld* _world)
{
  world = _world;

  setNodes();
}

void Dijkstra::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 1;

  if (searchState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchState = step();
  }
}

void Dijkstra::render()
{
  // render
  if (searchState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
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
  else if (searchState == SEARCHING_STATE::E::FOUND && elapsedFrames >= stepPerFrames)
  {
    showPath(targetCoord);
  }
}

void Dijkstra::run()
{
  if (false == isNodesSeted)
  {
    setNodes();
  }
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
      coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
      && world->getTiledMap()->getType(coordAux.x, coordAux.y) != 3)
    {
      weightAux = world->getTiledMap()->getCost(coordAux.x, coordAux.y) + node->weight;

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
      // agregamos el nodo a la lista abaierta, si es que no esta en la lista abierta o cerrada
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

  float nodeID = nextNodeID();
  closedNodes.push_back(openNodes[nodeID]); // agregamos a los nodos cerrados 
  openNodes.erase(openNodes.begin() + int(nodeID));// Remove the first element


  if (closedNodes[closedNodes.size() - 1]->coord == targetCoord)
  {
    return SEARCHING_STATE::FOUND;
  }
  else
  {
    addConnections(closedNodes[closedNodes.size() - 1]);
    return SEARCHING_STATE::SEARCHING;
  }
}
 