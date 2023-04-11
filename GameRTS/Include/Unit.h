#pragma once

#include "RTSUnitType.h"

#include "Externals/json.hpp"
#include <SFML/Graphics.hpp>

using RTSGame::RTSUnitType;

class Unit : public RTSUnitType
{
public:
  Unit() = default;
  ~Unit() = default;
  
  void 
  Init(sf::RenderTarget* m_pTarget);

  void
  Update(float deltaTime);

  void
  Render();

  void
  SetPosition(const Vector2& position)
  {
    m_texture.setPosition(position);
  }

private:
  Vector<RTSGame::AnimationFrame> animTest;

  uint32 CurrentFrame = 0;
  float framePerSecond = 0;

  float health = 1;
  float damage = 1;
  float movemetSpeed = 1;
};