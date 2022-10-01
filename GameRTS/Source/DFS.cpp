#include "DFS.h"
#include "RTSTiledMap.h"


void DFS::Init(RTSTiledMap* _tileMap)
{
  tileMap = _tileMap;
}

void DFS::update()
{
  if (canRun)
  {
    run(Vector2I(0, 0), Vector2I(250, 0));
    canRun = false;
  }
}

void DFS::run(const Vector2I& _startCoord, const Vector2I& _targetCoord)
{
  

  startCoord = _startCoord;
  targetCoord = _targetCoord;

  if (startCoord.x == NULL && startCoord.y == NULL &&
    targetCoord.x == NULL && targetCoord.y == NULL)
  {
    return;
  }

  openNodes.push_back(new Node(startCoord, nullptr));

  Vector2I nodeAux = { 0 , 0 };


  while (step() == false)
  {
    //consigue los siguientes nodos, en caso de no encotrar el target
    for (int i = 0; i < conections.nextNodes.size(); i++)
    {
      nodeAux = closedNodes[closedNodes.size() - 1]->coord + conections.nextNodes[i];

      if (nodeAux.x >= 0 && nodeAux.y >= 0 &&
        nodeAux.x < tileMap->getMapSize().x && nodeAux.y < tileMap->getMapSize().y
        && tileMap->getType(nodeAux.x, nodeAux.y) == 1)
      {
        // agregamos el nodo a la lista abaierta si es que no esta
        if (false == isInOpenList(nodeAux) && false == isInClosedList(nodeAux))
        {
          openNodes.push_back(new Node(nodeAux, closedNodes[closedNodes.size() - 1]));
        }
      }
    }
  }/**/
}

// checa el nodo siguiente de la lista abierta (ultimo)
bool DFS::step()
{

  //el nodo esta vacio
  if (openNodes.empty())
  {
    return true;
  }
  // econtro el nodo
  else if (openNodes[openNodes.size() -1]->coord == targetCoord)
  {
    closedNodes.push_back(openNodes[0]); // agregamos a los nodos cerrados 
    openNodes.erase(openNodes.begin());// Remove the first element

    showPath(targetCoord);
    return true;
  }
  // no es el nodo que buscamos
  else
  {

    closedNodes.push_back(openNodes[openNodes.size() - 1]); // agregamos a los nodos cerrados 
    tileMap->setType(openNodes[openNodes.size() - 1]->coord.x, openNodes[openNodes.size() - 1]->coord.y, 0);


    openNodes.pop_back();// Remove the last element

    return false;
  }
}
