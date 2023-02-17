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

  for (int i = 0; i < node->conections.nextNodes.size(); i++)
  {
    // ultimo nodo en meterse a la lista cerrada
    coordAux = node->coord + node->conections.nextNodes[i];

    // si esta adentro del mapa y su tipo no es obstaculo
    if (coordAux.x >= 0 && coordAux.y >= 0 &&
      coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
      && world->getTiledMap()->getType(coordAux.x, coordAux.y) != 3)
    {

      // si tiene una ruta mas barata
      if (true == isInClosedList(coordAux))
      {
        Node nodeInClosedList = *getNodeInClosedList(coordAux);
        Node newPath = Node(coordAux, node, 1, world->getTiledMap()->getCost(coordAux.x, coordAux.y));//diferent father

        if (newPath.weight < nodeInClosedList.weight)
        {
          getNodeInClosedList(coordAux)->fatherNode = node;
          getNodeInClosedList(coordAux)->setWeight(world->getTiledMap()->getCost(coordAux.x, coordAux.y));
        }
      }
      /*else if (true == isInOpenList(coordAux))
      {
        Node nodeInOpenList = *getNodeInOpenList(coordAux);
        Node newPath = Node(coordAux, node, 0); //diferent father

        if (newPath.distance < nodeInOpenList.distance)
        {
          getNodeInOpenList(coordAux)->fatherNode = node;
          getNodeInOpenList(coordAux)->setWeight(world->getTiledMap()->getCost(coordAux.x, coordAux.y));

        }
      }/**/

      // agregamos el nodo a la lista abaierta, si es que no esta en la lista abierta o cerrada
      if (false == isInOpenList(coordAux) && false == isInClosedList(coordAux))
      {
        openNodes.push_back(new Node(coordAux, node, 1, world->getTiledMap()->getCost(coordAux.x, coordAux.y)));
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
  openNodes.erase(openNodes.begin());// Remove the first element


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
 