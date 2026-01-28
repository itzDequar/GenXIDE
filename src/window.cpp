#include <iostream>
#include <window.h>

bool Window::init( const char* title, int w, int h, SDL_InitFlags windowFlags, SDL_InitFlags initFlags )
{
  bool success{ true };

  if ( SDL_Init(initFlags) != 1 )
  {
    std::cerr << "SDL Not Inited: " << SDL_GetError() << std::endl;
    success = false;
  }

  if ( window = SDL_CreateWindow(title, w, h, windowFlags); !window )
  {
    std::cerr << "SDL Window Not Created: " << SDL_GetError() << std::endl;
    success = false;
  }

  isRun = success;
  return success;
}

bool Window::destroy()
{
  SDL_DestroyWindow( window );
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
  }

  return true;
}

bool Window::input()
{
  SDL_Event event;
  while ( SDL_PollEvent(&event) )
  {
    if ( event.type == SDL_EVENT_QUIT )
    {
      isRun = false;
    }
  }

  return true;
}
