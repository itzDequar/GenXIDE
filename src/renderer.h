#pragma once
#include <SDL_syswm.h>
#include <SDL.h>
#include <vector>


class Renderer
{
  public:
  bool init( SDL_SysWMinfo wmi, int width, int height, int gridW, int gridH, std::vector<uint32_t>& pixels );
  bool destroy();

  void renderer( std::vector<uint32_t>& pixels );

  void sizeChanged( SDL_Event event, SDL_Window* window );


  private:
};
