#pragma once
#include <SDL_syswm.h>
#include <SDL.h>
#include <vector>
#include <glad/glad.h>

class Window;

struct Camera2D {
  float x = 0.0f;
  float y = 0.0f;
  float zoom = 1.0f;

  bool dragging = false;
  int lastMouseX = 0;
  int lastMouseY = 0;
};


class Renderer
{
  public:
  bool init( SDL_Window* window, int width, int height, int gridW, int gridH, std::vector<uint32_t>& pixels );
  bool destroy();

  void panZoom( Window& win );

  void renderer( std::vector<uint32_t>& pixels );

  void sizeChanged( SDL_Event event, SDL_Window* window );

  Camera2D cam{};
  private:
  SDL_Window* win{};
  GLuint program{};
  GLuint vao{}, vbo{}, ebo{};
  GLuint gridTex{};
  GLuint texLoc{};

  GLint uMvpLoc{ -1 };
  int gWidth{};
  int gHeight{};
  int wWidth{};
  int wHeight{};
};
