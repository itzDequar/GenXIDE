#include <iostream>
#include <renderer.h>

bool Renderer::init( SDL_SysWMinfo wmi, int width, int height, int gridW, int gridH )
{
  bool success{ true };

  return success;
}

bool Renderer::destroy()
{
  return true;
}

void Renderer::sizeChanged( SDL_Event event )
{
  if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
  {

  }
}


void Renderer::renderer()
{


  return;
}
