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

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

  if ( window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, windowFlags); !window )
  {
    std::cerr << "SDL Window Not Created: " << SDL_GetError() << std::endl;
    success = false;
  }

  glCtx = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, glCtx);
  SDL_GL_SetSwapInterval(1); // vsync

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cerr << "GLAD Not Loaded: " << SDL_GetError() << std::endl;
    success = false;
  }

  printf("GL: %s\n", glGetString(GL_VERSION));
  printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  isRun = success;
  return success;
}

bool Window::destroy()
{
  if ( window ) { SDL_DestroyWindow( window ); }
  SDL_Quit();
  return true;
}

bool Window::input()
{
  if ( event.type == SDL_QUIT )
  {
    isRun = false;
  }

  return true;
}
