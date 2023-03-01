#include "RTSPathMap.h"

#include "RTSTexture.h"

#include <geDebug.h>
#include <geColor.h>


RTSPathMap::RTSPathMap()
{
}

RTSPathMap::RTSPathMap(sf::RenderTarget* pTarget, const Vector2I& mapSize)
{
  init(pTarget, mapSize);
}

RTSPathMap::~RTSPathMap()
{
}

bool RTSPathMap::init(sf::RenderTarget* pTarget, const Vector2I& mapSize, String _textureRoute, Vector<String> _texturesNames)
{
  if (m_mapGrid.size()) {
    destroy();
  }

  m_pTarget = pTarget;

  m_mapGrid.resize(mapSize.x * mapSize.y);
  m_mapSize = mapSize;
  setCameraStartPosition(0, 0);
  
  m_mapTextures.resize(PATH_FTYPE::kNumObjects);

  String textureName;
  for (uint32 i = 0; i < PATH_FTYPE::kNumObjects; ++i) {
#ifdef MAP_IS_ISOMETRIC
    textureName = "Textures/PathType/iso_path_" + toString(i) + ".png";
#else
    textureName = "Textures/PathType/path_" + toString(i) + ".png";
#endif
    m_mapTextures[i].loadFromFile(m_pTarget, textureName);
  }

  preCalc();
  return true;
}
