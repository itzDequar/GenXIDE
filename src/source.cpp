#include <grid.h>
#include <renderer.h>
#include <iostream>
#include <unistd.h>
#include <window.h>

int main( int argc, char* args[] )
{
  char cwd[1024];
  getcwd(cwd, sizeof(cwd));
  std::cout << "Current working directory: " << cwd << std::endl;
  Window win{};
  Renderer ren{};
  Grid gri{};

  if ( !win.init( "GenXIDE", 900, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, SDL_INIT_VIDEO | SDL_INIT_EVENTS ) )
  {
    std::cerr << "FAILED TO INITIALIZE WINDOW." << std::endl;
    return 1;
  }
  if ( !gri.init( 100, 256, 128, 128, true) )
  {
    std::cerr << "FAILED TO INITIALIZE GRID." << std::endl;
    return 1;
  }
  if ( !ren.init( win.getWindow(), win.getWidth(), win.getHeight(), gri.getWidth(), gri.getHeight(), gri.getPixels() ) )
  {
    std::cerr << "FAILED TO INITIALIZE RENDERER." << std::endl;
    return 1;
  }

  while ( win.getIsRun() )
  {
    while ( SDL_PollEvent( &win.getEvent() ) )
    {
      win.input();
      ren.sizeChanged( win.getEvent(), win.getWindow() );
    }
    gri.update();
    ren.renderer( gri.getPixels() );
  }

  if ( !ren.destroy() )
  {
    std::cerr << "FAILED TO DESTROY BGFX." << std::endl;
    return 1;
  }
  if ( !win.destroy() )
  {
    std::cerr << "FAILED TO DESTROY SDL." << std::endl;
    return 1;
  }


  return 0;
}
