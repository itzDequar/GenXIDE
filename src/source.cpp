#include "grid.h"
#include "renderer.h"
#include <iostream>
#include <window.h>

int main( int argc, char* args[] )
{
  Window win{};
  Renderer ren{};
  Grid gri{};


  if ( !win.init( "GenXIDE", 900, 600, SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_RESIZABLE, SDL_INIT_VIDEO | SDL_INIT_EVENTS ) )
  {
    std::cerr << "FAILED TO INITIALIZE WINDOW." << std::endl;
    return 1;
  }
  if ( !ren.init() )
  {
    std::cerr << "FAILED TO INITIALIZE RENDERER." << std::endl;
    return 1;
  }
  if ( !gri.init( 100, 256, 128, 128, true) )
  {
    std::cerr << "FAILED TO INITIALIZE GRID." << std::endl;
    return 1;
  }

  if ( !win.run() )
  {
    std::cerr << "FAILED TO RUN SDL." << std::endl;
    return 1;
  }

  if ( !win.destroy() )
  {
    std::cerr << "FAILED TO DESTROY SDL." << std::endl;
    return 1;
  }


  return 0;
}
