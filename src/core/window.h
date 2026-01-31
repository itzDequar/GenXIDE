#pragma once
#include <SDL.h>
#include <glad/glad.h>
#include <string>

class Window
{
  public:
  Window() = default;
  ~Window();

  bool init( const char* title, int width, int height, Uint32 windowFlags, Uint32 initFlags );
  void destroy();
  void swapBuffers();

  inline bool isRunning() const { return m_isRunning; }
  inline int getWidth() const { return m_width; }
  inline int getHeight() const { return m_height; }
  inline SDL_Window* getSDLWindow() { return m_window; }
  inline SDL_GLContext getGLContext() { return m_glContext; }

  void getFramebufferSize( int& width, int& height );

  private:
  SDL_Window* m_window{ nullptr };
  SDL_GLContext m_glContext{ nullptr };

  int m_width{ 0 };
  int m_height{ 0 };
  bool m_isRunning{ false };
};
