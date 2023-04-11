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
      world->getPathTiledMap()->setType(openNodes[i]->coord.x, openNodes[i]->coord.y, PATH_FTYPE::E::kOpenList);
    }

    for (uint16 i = 0; i < closedNodes.size(); i++)
    {
      world->getPathTiledMap()->setType(closedNodes[i]->coord.x, closedNodes[i]->coord.y, PATH_FTYPE::E::kClosedList);
    }
    world->getPathTiledMap()->setType(startCoord->x, startCoord->y, PATH_FTYPE::E::kStart);
    world->getPathTiledMap()->setType(targetCoord->x, targetCoord->y, PATH_FTYPE::E::kTarget);

    elapsedFrames = 0;
  }
  else if (searchState == SEARCHING_STATE::E::FOUND && elapsedFrames >= stepPerFrames)
  {
    setPath();
    showPath();
  }

  rWindow->draw(lines);

}

void PathFinder::showPath()
{

  // --- draw line
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

  // Draw path
  for (uint32 i = 0; i < pathCoords.size(); i++)
  {
    world->getPathTiledMap()->setType(pathCoords[i].x, pathCoords[i].y, PATH_FTYPE::E::kPath);
  }

  /**/

}

bool once = false;

void
PathFinder::setPath()
{
  if (true == once )
  {
    return;
  }


  // Set default path by geting father by father
  Node* trgt = getNodeInClosedList(*targetCoord);
  Node* fatherNodeAux = trgt->fatherNode;
  while (fatherNodeAux != nullptr)
  {
    pathCoords.push_back(fatherNodeAux->coord);
    //world->getPathTiledMap()->setType(fatherNodeAux->coord.x, fatherNodeAux->coord.y, PATH_FTYPE::E::kPath);
    fatherNodeAux = fatherNodeAux->fatherNode;
  }


  bressenham();
  once = true;
}


void
PathFinder::bressenham()
{
  Vector2I currentCoords = pathCoords[0]; //first on the llist (target)

  geEngineSDK::Vector <Vector2I> savedCoords; // guarda el ultimo camino que si funciona
  geEngineSDK::Vector <Vector2I> newPath; // guarda el camino que se esta creando al momento

  Vector2I Pk = { 0,0 };
  float M = 0;

  for (uint32 i = 0; i < pathCoords.size() - 1; i++)
  {
    // es necesario cambiar de camino?

    Pk = currentCoords - pathCoords[i + 1];
    if (Pk.x == Pk.y || Pk.x == 0 || Pk.y == 0)
    {
      continue;
    }
    M = static_cast<float>(Pk.y) / static_cast<float>(Pk.x);

    // crea el camino de currentCoords a toCheckCoords
    Vector2I nextPoint = currentCoords;
    newPath.push_back(nextPoint);

    int nTimesToMove = (Pk.y < 0) ? -Pk.y : Pk.y;
    float nTimesToAdd = (M < 0) ? -M : M;
    bool isVertical = true;

    float auxX = (Pk.x < 0) ? -Pk.x : Pk.x;
    float auxY = (Pk.y < 0) ? -Pk.y : Pk.y;
    if (auxX < auxY) // la linea es mas vertical 
    {
      nTimesToMove = (Pk.x < 0) ? -Pk.x : Pk.x;
      isVertical = false;
    }
    else
    {
      M = static_cast<float>(Pk.x) / static_cast<float>(Pk.y);
      nTimesToAdd = (M < 0) ? -M : M;
    }

    uint32 addedcount = 1;
    Vector2I direction = { 1, 1 };
    direction.x = (Pk.x > 0) ? -1 : 1;
    direction.y = (Pk.y > 0) ? -1 : 1;
    bool canMakeNewPath = false;

    for (uint32 j = 1; j < nTimesToMove + 1; j++)
    {
      for (uint32 k = 1; k < static_cast<int>(nTimesToAdd) + 1; k++)
      {
        if (true == isVertical)
        {
          nextPoint.x = currentCoords.y + (addedcount * direction.x);
          nextPoint.y = currentCoords.x + (j * direction.y);

        }
        else
        {
          nextPoint.x = currentCoords.x + (j * direction.x);
          nextPoint.y = currentCoords.y + (addedcount * direction.y);
        }

        addedcount++;
        // mientras crea el camino checa que no sea un obstaculo
        canMakeNewPath =
          world->getGameMap()->getType(nextPoint.x, nextPoint.y) == TERRAIN_TYPE::kObstacle ||
          world->getGameMap()->getType(nextPoint.x, nextPoint.y) == TERRAIN_TYPE::kWater;

        if (true == canMakeNewPath) {
          break;
        }

        newPath.push_back(nextPoint);
      }

      // mientras crea el camino checa que no sea un obstaculo
      if (true == canMakeNewPath) {
        break;
      }

      nTimesToAdd = (M < 0) ?
        (nTimesToAdd - static_cast<int>(nTimesToAdd)) + -M :
        (nTimesToAdd - static_cast<int>(nTimesToAdd)) + M;
    }

    if (true == canMakeNewPath) {
      continue;
    }

    savedCoords = newPath;
    newPath.clear();
  }
  
  // si se creo un camino ...
  if (savedCoords.size() > 1)
  { 
    //encontrar de donde a donde se va a modificar
    for (uint32 i = 0; i < pathCoords.size() - 1; i++)
    { 

      if (pathCoords[i] == savedCoords[i])
      {
        for (uint32 j = 0; j < savedCoords.size() - 1; j++)
        {
          newPath.push_back(savedCoords[j]);
        }

        while (pathCoords[i] != savedCoords[pathCoords.size() - 1])
        {
          i++;
        }

      }

      newPath.push_back(pathCoords[i]);
    }

  }
 
  pathCoords = newPath;

  
}

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