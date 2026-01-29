#pragma once
#include <SDL.h>

class Window
{
  public:
  bool init( const char* title = "window", int w = 600, int h = 400, Uint32 windowFlags = 0, Uint32 initFlags = SDL_INIT_VIDEO );
  bool destroy();
  bool run();

  int getWidth() { return width; };
  int getHeight() { return height; };

  private:
  bool isRun{};

  SDL_Window* window{};

  int width{};
  int height{};

  bool input();
};
