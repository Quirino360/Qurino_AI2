#pragma once
#include <gePrerequisitesUtil.h>
#include <geVector2I.h>

class RTSWorld;

using geEngineSDK::Vector2I;
using geEngineSDK::uint32;

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

namespace SEARCHING_STATE
{
  enum E
  {
    NOT_SEARCHING = 0,
    SEARCHING,
    FOUND,
    NOT_FOUND,
    
  };
}


class Node
{
public:
  struct Conections
  {
    Conections() {
      SetDefaultNodes();
    }

    Conections(const geEngineSDK::Vector<Vector2I>& _nextNodes) {
      nextNodes = _nextNodes;
    }

    ~Conections()
    {
      nextNodes.empty();
    }

    void SetDefaultNodes() {

      nextNodes.empty();

      nextNodes.push_back(TOP);
      nextNodes.push_back(TOPRIGHT);
      nextNodes.push_back(RIGHT);
      nextNodes.push_back(DOWNRIGHT);
      nextNodes.push_back(DOWN);
      nextNodes.push_back(DOWNLEFT);
      nextNodes.push_back(LEFT);
      nextNodes.push_back(TOPLEFT);
    }

    void SetNextNodes(geEngineSDK::Vector<Vector2I>& _nextNodes) {
      nextNodes = _nextNodes;
    }

    void SetNextNode(Vector2I& newNode) {
      nextNodes.push_back(newNode);
    }

    geEngineSDK::Vector<Vector2I> nextNodes;

    static const Vector2I TOP;
    static const Vector2I TOPRIGHT;
    static const Vector2I TOPLEFT;
    static const Vector2I LEFT;
    static const Vector2I RIGHT;
    static const Vector2I DOWN;
    static const Vector2I DOWNRIGHT;
    static const Vector2I DOWNLEFT;
  };

  Node( const Vector2I& _coord, Node* _fatherNode, 
  const float& _distance = 0, const float& _weight = 0) {
    coord = _coord;
    fatherNode = _fatherNode;
    distance = _distance;
    if (fatherNode != nullptr)
    {
      weight = _weight + fatherNode->weight;
    }
  }

  ~Node() = default;

private:
  const Vector2I& getCoord() {
    return coord;
  }

  const Node& getFatherNode() {
    return *fatherNode;
  }

  const float& getDistance() {
    return distance;
  }

  const float& getWeight() {
    return weight;
  }

  void setCoord(const Vector2I& _coord) {
    coord = _coord;
  }

  void setFatherNode (Node* _fatherNode) {
    fatherNode = _fatherNode;
  }

  void setDistance(const float& _distance) {
    distance = _distance;
  }

  void setWeight(const float& _weight) {
    weight = _weight;
  }

public:
  Conections conections = Conections();
  Vector2I coord;
  Node* fatherNode = nullptr;
  float distance;
  float weight; // el peso tiene que ir en el nodo de revision

  //en best necesito una euristica para calcular esto, en este caso es la distancia
  
  /*float getTotalWeight() {
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
  }/**/

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
    
  }

protected:
  // main algorithm
  virtual void
  run() = 0;

  // search for new nodes and 
  virtual SEARCHING_STATE::E
  step() = 0;

  virtual uint32
  nextNodeID() = 0;

  virtual void
  addConnections(const Node& node) {};

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

  float stepPerFrames = 1;

  float elapsedFrames = 0;

  bool isNodesSeted = false;

  SEARCHING_STATE::E searchingState = SEARCHING_STATE::E::SEARCHING;
};


//5 PathFinders
// Escoger entre los 5 algoritmos en una UI
// Poder setear pesos, con el ejemplo que vimos
// Interfaz de control
//_tiledMap->getType
