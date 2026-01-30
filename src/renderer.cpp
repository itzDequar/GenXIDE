#include <iostream>
#include <renderer.h>

bool Renderer::init( SDL_SysWMinfo wmi, int width, int height, int gridW, int gridH, std::vector<uint32_t>& pixels )
{
  bool success{ true };

  return success;
}

bool Renderer::destroy()
{
  return true;
}

void Renderer::sizeChanged( SDL_Event event, SDL_Window* window )
{
  if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    // int w, h;
    // SDL_GetWindowSize(window, &w, &h);
  }
}


void Renderer::renderer( std::vector<uint32_t>& pixels )
{

  return;
}
