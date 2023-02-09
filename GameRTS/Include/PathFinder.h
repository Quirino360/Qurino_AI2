#pragma once
#include <gePrerequisitesUtil.h>
#include <geVector2I.h>

class RTSWorld;

using geEngineSDK::Vector2I;

// cambiarlo a clases, totalmente n herencia
/*
  NONE = -1,
  BreadthFirstSearch,
  DepthFirstSearch,
  BEST,
  DIJKSTRA,
  ASTAR
  /**/


// una vez revisa hace el camino
// Una herramienta que se va usar en el mapa, sin modificar el mapa 

class Node
{
public:
  
  Node(Vector2I& _coord, Node* _fatherNode, float _weight) {
    coord = _coord;
    fatherNode = _fatherNode;
    weight = _weight;
  }

  ~Node() = default;

  Vector2I coord;
  Node* fatherNode = nullptr;
  float weight;

  float getTotalWeight() {
    float _weight = weight;
    bool searchFather = true;
    Node* auxNode = fatherNode;

    while (true == searchFather)
    {
      if (auxNode != nullptr)
      {
        _weight += auxNode->weight;
        auxNode = auxNode->fatherNode;
      }
      else
      {
        searchFather = false;
      }
    }

    return _weight;
  }

};

struct Conections
{
  Conections() {
    SetDefaultNodes();
  }

  ~Conections()
  {
    nextNodes.empty();
  }
  
  void SetDefaultNodes() {
    
    nextNodes.empty();

    nextNodes.push_back(top);
    nextNodes.push_back(topRight);
    nextNodes.push_back(right);
    nextNodes.push_back(downRight);
    nextNodes.push_back(down);
    nextNodes.push_back(downLeft);
    nextNodes.push_back(left);
    nextNodes.push_back(topLeft);
  }

  void SetNextNodes(geEngineSDK::Vector<Vector2I>& _nextNodes) {
    nextNodes = _nextNodes;
  }

  void SetNextNodes(Vector2I& newNode) {
    nextNodes.push_back(newNode);
  }

  geEngineSDK::Vector<Vector2I> nextNodes;
 
  const Vector2I top =        Vector2I( 0,-1 );
  const Vector2I topRight =   Vector2I( 1,-1 );
  const Vector2I topLeft =    Vector2I(-1,-1 );
  const Vector2I left =       Vector2I(-1, 0 );
  const Vector2I right =      Vector2I( 1, 0 );
  const Vector2I down =       Vector2I( 0, 1 );
  const Vector2I downRight =  Vector2I( 1, 1 );
  const Vector2I downLeft =   Vector2I(-1, 1 );
};

//textureNames = {"Untiled", "Start", "Target", "OpenList", "ClosedList" };

class PathFinder
{
public:
  PathFinder();
  ~PathFinder() = default;

  virtual void
  Init(RTSWorld* _world) = 0;

  virtual void
  update(float deltaTime);

  virtual void
  render() = 0;

public:
  
  virtual void
  setNodes(const Vector2I& _startCoord = Vector2I(0, 0),
    const Vector2I& _targetCoord = Vector2I(10, 10));

  void 
  showPath(Vector2I _target);

  void 
  startSearching() {
    if (isFindingTarget == false)
    {
      isFindingTarget = true;
    }
  }

protected:
  // main algorithm
  virtual void
  run() = 0;

  // search for new nodes and 
  virtual bool
  step() = 0;

  bool isInOpenList(Vector2I _target);

  bool isInClosedList(Vector2I _target);

  Node* getNodeInClosedList(Vector2I _target);

  virtual void
  resetPath();

protected:
  RTSWorld* world;

  geEngineSDK::Vector<Node*> openNodes;

  geEngineSDK::Vector<Node*> closedNodes;

  Vector2I startCoord;

  Vector2I targetCoord;

  Conections conections;

  float stepPerFrames = 1;

  float elapsedFrames = 0;

  bool isNodesSeted = false;

  bool isFindingTarget = false;
};


//5 PathFinders
// Escoger entre los 5 algoritmos en una UI
// Poder setear pesos, con el ejemplo que vimos
// Interfaz de control
//_tiledMap->getType
