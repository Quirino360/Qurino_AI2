#pragma once

#include "RTSTiledMap.h"

namespace PATH_FTYPE {
  enum E {
    kUntiled = 0,
    kStart,
    kTarget,
    kOpenList,
    kClosedList,
    kPath,
    kNumObjects
  };
}

class RTSPathMap : public RTSTiledMap
{
public:
  RTSPathMap();
  RTSPathMap(sf::RenderTarget* pTarget, const Vector2I& mapSize);
  ~RTSPathMap();


  virtual bool
  init(sf::RenderTarget* pTarget, const Vector2I& mapSize,
    String  _textureRoute = "Textures/Terrain",
    Vector<String> _texturesNames = Vector<String>());

};