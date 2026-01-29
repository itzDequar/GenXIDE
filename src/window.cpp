#include "SDL_events.h"
#include "SDL_video.h"
#include <iostream>
#include <window.h>

bool Window::init( const char* title, int w, int h, Uint32 windowFlags, Uint32 initFlags )
{
  bool success{ true };

  width = w;
  height = h;

  if ( SDL_Init(initFlags) < 0 )
  {
    std::cerr << "SDL Not Inited: " << SDL_GetError() << std::endl;
    success = false;
  }

  if ( window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, windowFlags); !window )
  {
    std::cerr << "SDL Window Not Created: " << SDL_GetError() << std::endl;
    success = false;
  }

  isRun = success;
  return success;
}

bool Window::destroy()
{
  if ( window ) { SDL_DestroyWindow( window ); }
  SDL_Quit();
  return true;
}

bool Window::run()
{
  if ( !isRun )
  {
    return false;
  }

  while ( isRun )
  {
    input();
    SDL_Delay(16);
  }

  return true;
}

bool Window::input()
{
  SDL_Event event;
  while ( SDL_PollEvent(&event) )
  {
    if ( event.type == SDL_QUIT )
    {
      isRun = false;
    }
  }

  return true;
}
