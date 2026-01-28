#pragma once
#include <SDL3/SDL.h>

class Window
{
  public:
  bool init( const char* title = "window", int w = 600, int h = 400, SDL_InitFlags windowFlags = 0, SDL_InitFlags initFlags = SDL_INIT_VIDEO );
  bool destroy();
  bool run();


  private:
  bool isRun{};
  SDL_Window* window{};

  bool input();
};
