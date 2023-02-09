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

  if (true == isFindingTarget)
  {
    if (elapsedFrames >= stepPerFrames)
    {
      run();
    }
  }
}

void Best::render()
{
  // render
  if (true == isFindingTarget)
  {
    if (elapsedFrames >= stepPerFrames)
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
}

void Best::run()
{

  if (false == isNodesSeted)
  {
    setNodes();
  }

  //openNodes.push_back(new Node(startCoord, nullptr));

  Vector2I coordAux = { 0 , 0 };

  if (isFindingTarget == true)
  {
    step();

    //consigue los siguientes nodos, en caso de no encotrar el target
    for (int i = 0; i < conections.nextNodes.size(); i++)
    {
      // ultimo nodo en meterse a la lista cerrada
      coordAux = closedNodes[closedNodes.size() - 1]->coord + conections.nextNodes[i];

      // si esta adentro del mapa y su tipo no es obstaculo
      if (coordAux.x >= 0 && coordAux.y >= 0 &&
        coordAux.x < world->getTiledMap()->getMapSize().x && coordAux.y < world->getTiledMap()->getMapSize().y
        && world->getTiledMap()->getType(coordAux.x, coordAux.y) != 3)
      {

        //* falata checar que cuando encuentre una nueva manera de llegar a un nodo, si es ma barato, en caso de que si asigar una nuevo padre *//
        if (true == isInClosedList(coordAux))
        {
          Node nodeInClosedList = *getNodeInClosedList(coordAux);
          Node newPath = Node(coordAux, closedNodes[closedNodes.size() - 1], world->getTiledMap()->getCost(coordAux.x, coordAux.y));//diferent father

          if ( newPath.getTotalWeight() < nodeInClosedList.getTotalWeight())
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


    // ver cual es la ruta mas barata
    float cheapest = 999999999999;
    float totalWeight = 0;

    for (uint32 i = 0; i < openNodes.size(); i++)
    {
      totalWeight = openNodes[i]->getTotalWeight();
      if (totalWeight < cheapest)
      {
        cheapest = totalWeight;
        nextToCheck = i;
      };

    }

    if (isFindingTarget == false)
    {
      for (uint16 i = 0; i < openNodes.size(); i++)
      {
        world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 3);
      }
      for (uint16 i = 0; i < closedNodes.size(); i++)
      {
        world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 4);
      }

      showPath(targetCoord);
    }

  }
}

// checa el nodo siguiente de la lista abierta (primero)
bool Best::step()
{

  //el nodo esta vacio
  if (openNodes.empty())
  {
    isFindingTarget = false; // ya no hay nada que buscar
    return true;
  }
  // econtro el nodo
  else if (openNodes[nextToCheck]->coord == targetCoord)
  {
    isFindingTarget = false; // ya no hay nada que buscar

    closedNodes.push_back(openNodes[nextToCheck]); // agregamos a los nodos cerrados 
    openNodes.erase(openNodes.begin());// Remove the first element

    //showPath(targetCoord);
    return true;
  }
  // no es el nodo que buscamos
  else
  {
    closedNodes.push_back(openNodes[nextToCheck]); // agregamos a los nodos cerrados 
    //world->getPathTiledMap()->setType(openNodes[0]->coord.x, openNodes[0]->coord.y, 0);

    openNodes.erase(openNodes.begin());// Remove the first element

    return false;
  }
}
