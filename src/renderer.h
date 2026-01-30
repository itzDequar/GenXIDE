#pragma once
#include <SDL_syswm.h>
#include <SDL.h>

class Renderer
{
  public:
  bool init( SDL_SysWMinfo wmi, int width, int height, int gridW, int gridH );
  bool destroy();

  void renderer();

  void sizeChanged( SDL_Event event );


  private:
};
