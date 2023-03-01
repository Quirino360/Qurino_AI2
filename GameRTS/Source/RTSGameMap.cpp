#include "RTSGameMap.h"

#include "RTSTexture.h"

#include <geDebug.h>
#include <geColor.h>


RTSGameMap::RTSGameMap()
{

}

RTSGameMap::RTSGameMap(sf::RenderTarget* pTarget, const Vector2I& mapSize)
{
  init(pTarget, mapSize);
}

RTSGameMap::~RTSGameMap()
{
}

bool RTSGameMap::init(sf::RenderTarget* pTarget, const Vector2I& mapSize, String _textureRoute, Vector<String> _texturesNames)
{
  if (m_mapGrid.size()) {
    destroy();
  }

  m_pTarget = pTarget;

  m_mapGrid.resize(mapSize.x * mapSize.y);
  m_mapSize = mapSize;
  setCameraStartPosition(0, 0);

  m_mapTextures.resize(TERRAIN_TYPE::kNumObjects);

  String textureName;
  for (uint32 i = 0; i < TERRAIN_TYPE::kNumObjects; ++i) {
#ifdef MAP_IS_ISOMETRIC
    textureName = "Textures/Terrain/iso_terrain_" + toString(i) + ".png";
#else
    textureName = "Textures/Terrain/terrain_" + toString(i) + ".png";
#endif
    m_mapTextures[i].loadFromFile(m_pTarget, textureName);
  }

  preCalc();
  return true;
}

int8
RTSGameMap::getCost(const int32 x, const int32 y) const {
  GE_ASSERT((x >= 0) && (x < m_mapSize.x) && (y >= 0) && (y < m_mapSize.y));
  return m_mapGrid[(y * m_mapSize.x) + x].getCost();
}

void
RTSGameMap::setCost(const int32 x, const int32 y, const int8 cost) {
  GE_ASSERT((x >= 0) && (x < m_mapSize.x) && (y >= 0) && (y < m_mapSize.y));
  m_mapGrid[(y * m_mapSize.x) + x].setCost(cost);
}

bool
RTSGameMap::loadFromImageFile(sf::RenderTarget* pTarget, String fileName) {
  sf::Image image;


  if (!image.loadFromFile(fileName.c_str())) {
    LOGWRN("File not found: " + fileName);
    return false;
  }

  if (!init(pTarget, Vector2I(image.getSize().x, image.getSize().y))) {
    LOGERR("Failed initializing map data");
    return false;
  }

  //With this library, the pixels always are 4 bytes in length
  int32 lineBytes = image.getSize().x * 4;
  const uint8* pPixeles = reinterpret_cast<const uint8*>(image.getPixelsPtr());

  for (int32 tmpY = 0; tmpY < m_mapSize.y; ++tmpY) {
    for (int32 tmpX = 0; tmpX < m_mapSize.x; ++tmpX) {
      uint8 tipoTerreno = TERRAIN_TYPE::kObstacle;
      Color actualColor(
        pPixeles[(tmpY * lineBytes) + (tmpX * 4) + 0],
        pPixeles[(tmpY * lineBytes) + (tmpX * 4) + 1],
        pPixeles[(tmpY * lineBytes) + (tmpX * 4) + 2],
        pPixeles[(tmpY * lineBytes) + (tmpX * 4) + 3]);

      if (Color::Blue == actualColor) {
        tipoTerreno = TERRAIN_TYPE::kWater;
      }
      else if (Color::Green == actualColor) {
        tipoTerreno = TERRAIN_TYPE::kGrass;
      }
      else if (Color::Yellow == actualColor) {
        tipoTerreno = TERRAIN_TYPE::kMarsh;
      }

      setType(tmpX, tmpY, tipoTerreno);
    }
  }
  return true;

  /**/

  return false; //delete this if fixed
}

bool
RTSGameMap::saveToImageFile(sf::RenderTarget*, String fileName) {
  return false;
}
