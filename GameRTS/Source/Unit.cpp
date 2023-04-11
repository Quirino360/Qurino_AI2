#include "Unit.h"

void 
Unit::Init(sf::RenderTarget* m_pTarget)
{
  loadAnimationData(m_pTarget, 1);
  float a = 0;
  animTest = m_animationFrames[0].frames[0];
  m_texture.setSrcRect(animTest[0].x, animTest[0].y, animTest[0].w, animTest[0].w);

}

void 
Unit::Update(float deltaTime)
{
  framePerSecond += deltaTime;
  //std::cout << framePerSecond << std::endl;
  if (framePerSecond >= 0.1)
  {
    if (CurrentFrame >= animTest.size())
    {
      CurrentFrame = 0;
    }

    m_texture.setSrcRect(animTest[CurrentFrame].x, animTest[CurrentFrame].y, animTest[CurrentFrame].w, animTest[CurrentFrame].w);
    CurrentFrame += 1;
    framePerSecond = 0;
  }
}

void
Unit::Render()
{
  m_texture.draw();
}