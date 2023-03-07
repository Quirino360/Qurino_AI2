#include "PathFinder.h"

#include "RTSWorld.h"
#include "RTSTiledMap.h"
#include "RTSPathMap.h"
#include "RTSGameMap.h"




 const Vector2I Node::Conections::TOP = Vector2I(0, -1);
 const Vector2I Node::Conections::TOPRIGHT = Vector2I(1, -1);
 const Vector2I Node::Conections::TOPLEFT = Vector2I(-1, -1);
 const Vector2I Node::Conections::LEFT = Vector2I(-1, 0);
 const Vector2I Node::Conections::RIGHT = Vector2I(1, 0);
 const Vector2I Node::Conections::DOWN = Vector2I(0, 1);
 const Vector2I Node::Conections::DOWNRIGHT = Vector2I(1, 1);
 const Vector2I Node::Conections::DOWNLEFT = Vector2I(-1, 1);
 


PathFinder::PathFinder()
{

}

void PathFinder::Init(RTSWorld* _world, Vector2I& _startCoord, Vector2I& _targetCoord)
{
  world = _world;

  startCoord = &_startCoord;
  targetCoord = &_targetCoord;

  setStartCoord(*startCoord);
  setTargetCoord(*targetCoord);

}

void PathFinder::update(float deltaTime)
{
  // show step by step
  elapsedFrames += 1;

  if (searchState == SEARCHING_STATE::E::SEARCHING && elapsedFrames >= stepPerFrames)
  {
    searchState = step();
  }
}

// cambiar esto, en parte de que se ande imprimiendo cada frame que se cambie el tipo solo cuando sea necesario
void PathFinder::render(sf::RenderWindow* rWindow)
{
  // render
  if (searchState != SEARCHING_STATE::E::FOUND && elapsedFrames >= stepPerFrames)
  {
    for (uint16 i = 0; i < openNodes.size(); i++)
    {
      world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 3);
    }

    for (uint16 i = 0; i < closedNodes.size(); i++)
    {
      world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 4);
    }
    world->getPathTiledMap()->setType(startCoord->x, startCoord->y, 1);
    world->getPathTiledMap()->setType(targetCoord->x, targetCoord->y, 2);

    elapsedFrames = 0;
  }
  else if (searchState == SEARCHING_STATE::E::FOUND && elapsedFrames >= stepPerFrames)
  {
    showPath(*targetCoord);
  }

  rWindow->draw(lines);

}

void PathFinder::showPath(Vector2I _target)
{
  geEngineSDK::Vector <Vector2I> pathCoords;
  Node* trgt = getNodeInClosedList(_target);
  Node* fatherNodeAux = trgt->fatherNode;


  pathCoords.push_back(*targetCoord);
  world->getPathTiledMap()->setType(trgt->coord.x, trgt->coord.y, 5);

  while (fatherNodeAux != nullptr)
  {
    pathCoords.push_back(fatherNodeAux->coord);
    world->getPathTiledMap()->setType(fatherNodeAux->coord.x, fatherNodeAux->coord.y, 5);
    fatherNodeAux = fatherNodeAux->fatherNode;
  }


  // ------------------- Bresenham Algotithm // metorlo en funcion
  Vector2I currentCoords = { pathCoords[pathCoords.size() - 1]}; //first on the llist (target)
  Vector2I toCheckCoords = { pathCoords[pathCoords.size() - 2] }; //second on the list 

  geEngineSDK::Vector <Vector2I> savedCoords = { currentCoords };
  geEngineSDK::Vector <Vector2I> newPath;

  Vector2I Pk = currentCoords - toCheckCoords;
  float M = Pk.y / Pk.x;
  for (uint32 i = 0; i < pathCoords.size(); i++)
  {
    // es necesario cambiar de camino?
    if (Pk.x == Pk.y) //|| Pk.x == 0 || Pk.y == 0  
    {
      continue;
    }

    Vector2I nextPoint = currentCoords;
    newPath.push_back(currentCoords);

    int nTimesToMove = (Pk.x < 0) ? -Pk.x : Pk.x;
    int nTimesToAdd = (M < 0) ? -M : M;
    bool isUp = true;

    if (M > 1) // la linea es mas vertical 
    {
      nTimesToMove = (Pk.y < 0) ? -Pk.y : Pk.y;
      isUp = false;
    }

    for (uint32 j = 0; j < nTimesToMove; j++)
    {
      for (uint32 k = 0; k < nTimesToAdd; k++)
      {
        if (isUp)
        {
          nextPoint.x += currentCoords.x + j;
          nextPoint.y += currentCoords.y + k;
        }
        else
        {
          nextPoint.x += currentCoords.x + k;
          nextPoint.y += currentCoords.y + j;
        }
        newPath.push_back(nextPoint);
      }
      M = (M - static_cast<int>(M)) + Pk.y / Pk.x;
      nTimesToAdd = (M < 0) ? -M : M;
    }

  }


  // --- draw Bresenham Algotithm (SFML)
  lines = sf::VertexArray(sf::LinesStrip, pathCoords.size());

  Vector2I topLeft, buttomRight, center;
  world->getPathTiledMap()->getMapToScreenCoords(0, 0, topLeft.x, topLeft.y);
  world->getPathTiledMap()->getMapToScreenCoords(1, 1, buttomRight.x, buttomRight.y);
  center = (buttomRight - topLeft) / 2;
  sf::Vector2i tileCenter = sf::Vector2i(center.x, center.y);

  sf::Vector2i ScreenPos;
  for (uint32 i = 0; i < pathCoords.size(); i++)
  {
    world->getPathTiledMap()->getMapToScreenCoords(pathCoords[i].x, pathCoords[i].y, ScreenPos.x, ScreenPos.y);
    ScreenPos += tileCenter;
    lines[i].position = static_cast<sf::Vector2f>(ScreenPos);

  }

}

/*


*/

void PathFinder::setStartCoord(const Vector2I coord)
{
  world->getPathTiledMap()->setType(startCoord->x, startCoord->y, 0);
  *startCoord = coord;

  openNodes.clear();
  auto dist = *targetCoord - *startCoord;
  openNodes.push_back(new Node(*startCoord, nullptr, dist.size(), 0));
}

void PathFinder::setTargetCoord(const Vector2I coord)
{ 
  world->getPathTiledMap()->setType(targetCoord->x, targetCoord->y, 0);
  *targetCoord = coord;
  
  openNodes.clear();
  auto dist = *targetCoord - *startCoord;
  openNodes.push_back(new Node(*startCoord, nullptr, dist.size(), 0));
}

// como optimzar, para despues
Node* PathFinder::getNodeInClosedList(Vector2I _target)
{
  for (int i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i]->coord == _target)
    {
      return closedNodes[i];
    }
  }
  return nullptr;
}

Node* PathFinder::getNodeInOpenList(Vector2I _target)
{
  for (int i = 0; i < openNodes.size(); i++)
  {
    if (openNodes[i]->coord == _target)
    {
      return openNodes[i];
    }
  }
  return nullptr;
}

void PathFinder::resetPath()
{
  for (uint32 i = 0; i < openNodes.size(); i++)
  {
    if (openNodes[i] != nullptr)
    {
      world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, 0);
      delete openNodes[i];
    }
  }
  openNodes.clear();

  for (uint32 i = 0; i < closedNodes.size(); i++)
  {
    if (closedNodes[i] != nullptr)
    {
      world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, 0);
      delete closedNodes[i];
    }
  }
  closedNodes.clear();

  searchState = SEARCHING_STATE::NOT_SEARCHING;

  setStartCoord(*startCoord);
  setTargetCoord(*targetCoord);

}



// explorar los nodos
// checar que sea el nodo que buscamos
// guardar los nodos 
// escoger el siguiente nodo (esto es lo que cambia)


// *Crear una lista por los cuales fui avanzando?*