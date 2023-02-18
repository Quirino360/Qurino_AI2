#include "Astar.h"
#include "RTSWorld.h"
#include "RTSTiledMap.h"
void Astar::Init(RTSWorld* _world)
{
  world = _world;

  setNodes();
}

void Astar::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 1;

  if (searchState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchState = step();
  }
}

void Astar::render()
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

void Astar::run()
{
  if (false == isNodesSeted)
  {
    setNodes();
  }
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
  float distanceAux = 0;
  float weightAux = 0;

  for (int i = 0; i < node->conections.nextNodes.size(); i++)
  {
    // ultimo nodo en meterse a la lista cerrada
    coordAux = node->coord + node->conections.nextNodes[i];

    /*if (coordAux == Node::Conections::TOP + node->coord || coordAux == Node::Conections::RIGHT + node->coord ||
      coordAux == Node::Conections::DOWN + node->coord || coordAux == Node::Conections::LEFT + node->coord)
    {
      distanceAux = 1;
    }
    else 
    {
      distanceAux = 1.5;
    }*/
    auto dist = targetCoord - coordAux;
    distanceAux = dist.size();

    // si esta adentro del mapa y su tipo no es obstaculo
    if (coordAux.x >= 0 && coordAux.y >= 0 &&
      coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
      && world->getTiledMap()->getType(coordAux.x, coordAux.y) == 1)
    {
      //

      Node* nodeInClosedList = getNodeInClosedList(coordAux);
      if (nullptr != nodeInClosedList)
      {
        //Node newPath = Node(coordAux, node, distanceAux);//diferent father

        // si tiene una ruta mas corta, se cambia la ruta
        if (distanceAux < nodeInClosedList->distance)
        {
          nodeInClosedList->fatherNode = node;
          nodeInClosedList->setDistance(distanceAux);
        }
      }
      /*else if (true == isInOpenList(coordAux))
      {
        Node nodeInOpenList = *getNodeInOpenList(coordAux);
        Node newPath = Node(coordAux, node, distanceAux); //diferent father

        if (newPath.distance < nodeInOpenList.distance)
        {
          getNodeInOpenList(coordAux)->fatherNode = node;
          getNodeInOpenList(coordAux)->setDistance(distanceAux);

        }
      }/**/

      // agregamos el nodo a la lista abaierta, si es que no esta en la lista abierta o cerrada
      else if (nullptr == getNodeInOpenList(coordAux))
      {
        openNodes.push_back(new Node(coordAux, node, distanceAux));
      }
    }
  
  }
}

SEARCHING_STATE::E Astar::step()
{
  return SEARCHING_STATE::E();
}
