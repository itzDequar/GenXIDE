#pragma once
#include <SDL.h>
#include <SDL_syswm.h>
#include <renderer.h>

class Window
{
  public:
  bool init( const char* title = "window", int w = 600, int h = 400, Uint32 windowFlags = 0, Uint32 initFlags = SDL_INIT_VIDEO );
  bool destroy();
  bool input();

  bool getIsRun() { return isRun; };
  SDL_Event& getEvent() { return event; };
  SDL_SysWMinfo getWmi() { return wmi; };
  int getWidth() { return width; };
  int getHeight() { return height; };
  SDL_Window* getWindow() { return window; };

  private:
  bool isRun{};

  SDL_Event event;

  SDL_Window* window{};
  SDL_SysWMinfo wmi{};

  int width{};
  int height{};
};
