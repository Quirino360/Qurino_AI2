#include "Best.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"


void Best::Init(RTSWorld* _world)
{
  world = _world;

  setNodes();
}

void Best::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 1;

  if (searchingState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchingState = step();
  }
}

void Best::render()
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

void Best::run()
{
  if (false == isNodesSeted)
  {
    setNodes();
  }

  if (step() != SEARCHING_STATE::FOUND)
  { 
    showPath(targetCoord);
  }
}

uint32 Best::nextNodeID()
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


void Best::addConnections(const Node& node)
{
  //adicionar los nodos a la lista de posibilidades (funcion)
  //consigue los siguientes nodos, en caso de no encotrar el target
  Vector2I coordAux = { 0 , 0 };

  for (int i = 0; i < node.conections.nextNodes.size(); i++)
  {
    // ultimo nodo en meterse a la lista cerrada
    coordAux = closedNodes[closedNodes.size() - 1]->coord + node.conections.nextNodes[i];

    // si esta adentro del mapa y su tipo no es obstaculo
    if (coordAux.x >= 0 && coordAux.y >= 0 &&
      coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
      && world->getTiledMap()->getType(coordAux.x, coordAux.y) != 3)
    {

      // si tiene una ruta mas barata
      if (true == isInClosedList(coordAux))
      {
        Node nodeInClosedList = *getNodeInClosedList(coordAux);
        Node newPath = Node(coordAux, closedNodes[closedNodes.size() - 1], world->getTiledMap()->getCost(coordAux.x, coordAux.y));//diferent father

        if (newPath.weight < nodeInClosedList.weight)
        {
          getNodeInClosedList(coordAux)->fatherNode = closedNodes[closedNodes.size() - 1];
        }
      }

      // agregamos el nodo a la lista abaierta, si es que no esta en la lista abierta o cerrada
      if (false == isInOpenList(coordAux) && false == isInClosedList(coordAux))
      {
        openNodes.push_back(new Node(coordAux, closedNodes[closedNodes.size() - 1], world->getTiledMap()->getCost(coordAux.x, coordAux.y)));
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
}

//que tengo,limpio las listas, recibo un nodo inicial y el de buscar
// el nodo inicicial, meterla en la lista de posibilidades (setup)
// dar un paso, buscar en la lista, si esta vacia, ya valio, 