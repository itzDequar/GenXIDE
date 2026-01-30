#pragma once
#include <SDL2/SDL_syswm.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <SDL.h>

class Renderer
{
  public:
  bool init( SDL_SysWMinfo wmi, int width, int height );
  bool destroy();

  void renderer();

  void sizeChanged( SDL_Event event );


  private:
  bgfx::PlatformData pd{};
};
