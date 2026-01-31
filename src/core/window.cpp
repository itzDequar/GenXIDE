#include "window.h"
#include "core/config.h"
#include <iostream>

Window::~Window()
{
  destroy();
}

bool Window::init( const char* title, int width, int height, Uint32 windowFlags, Uint32 initFlags )
{
  m_width = width;
  m_height = height;

  if ( SDL_Init(initFlags) < 0 )
  {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
    return false;
  }

  // OpenGL 3.3 Core Profile
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

  m_window = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width,
    height,
    windowFlags
  );

  if ( !m_window )
  {
    std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return false;
  }

  m_glContext = SDL_GL_CreateContext(m_window);
  if ( !m_glContext )
  {
    std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return false;
  }

  SDL_GL_MakeCurrent(m_window, m_glContext);
  SDL_GL_SetSwapInterval(Config::ENABLE_VSYNC); // VSync

  if ( !gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) )
  {
    std::cerr << "GLAD initialization failed" << std::endl;
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return false;
  }

  m_isRunning = true;
  return true;
}

void Window::destroy()
{
  if ( m_glContext )
  {
    SDL_GL_DeleteContext(m_glContext);
    m_glContext = nullptr;
  }

  if ( m_window )
  {
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
  }

  SDL_Quit();
  m_isRunning = false;
}

void Window::swapBuffers()
{
  SDL_GL_SwapWindow(m_window);
}

void Window::getFramebufferSize( int& width, int& height )
{
  SDL_GL_GetDrawableSize(m_window, &width, &height);
}
