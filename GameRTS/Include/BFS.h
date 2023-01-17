#pragma once
#include "PathFinder.h"

class RTSTiledMap;

class BFS : public PathFinder
{
public:
  BFS() = default;

  ~BFS() = default;

  virtual void 
  Init(RTSTiledMap* _tileMap);

  virtual void
  update(float deltaTime);

  // main algorithm
  virtual void
  run(const Vector2I& _startCoord, const Vector2I& _targetCoord);

  void
  runStepByStep(const Vector2I& _startCoord, const Vector2I& _targetCoord);

  // search for new nodes and 
  virtual bool
  step();

  void 
  setNodes(const Vector2I& _startCoord = Vector2I(0,0), 
    const Vector2I& _targetCoord = Vector2I(10, 10));

private:

  Conections conections;

  bool isTargetFounded = false;

  bool isStepByStep = true;

  bool isNodesSeted = false;

  float timePerStep = 1;

  float elapsedTimePerStep = 1;
};

