#pragma once

#include "RTSTiledMap.h"

namespace TERRAIN_TYPE {
  enum E {
    kWater = 0,
    kGrass,
    kMarsh,
    kObstacle,
    kNumObjects
  };
}


class RTSGameMap : public RTSTiledMap
{
public:
  RTSGameMap();
  RTSGameMap(sf::RenderTarget* pTarget, const Vector2I& mapSize);
  ~RTSGameMap();


  virtual bool
  init(sf::RenderTarget* pTarget, const Vector2I& mapSize,
    String  _textureRoute = "Textures/Terrain",
    Vector<String> _texturesNames = Vector<String>());

  int8
  getCost(const int32 x, const int32 y) const;

  void
  setCost(const int32 x, const int32 y, const int8 cost);

  bool
  loadFromImageFile(sf::RenderTarget* pTarget, String fileName);

  bool
  saveToImageFile(sf::RenderTarget* pTarget, String fileName);
};