#pragma once
#include "SDL_video.h"
#include <SDL_syswm.h>
#include <SDL.h>
#include <vector>
#include <glad/glad.h>


class Renderer
{
  public:
  bool init( SDL_Window* window, int width, int height, int gridW, int gridH, std::vector<uint32_t>& pixels );
  bool destroy();

  void renderer( std::vector<uint32_t>& pixels );

  void sizeChanged( SDL_Event event, SDL_Window* window );


  private:
  SDL_Window* win{};
  GLuint program{};
  GLuint vao{}, vbo{}, ebo{};
  GLuint gridTex{};
  GLuint texLoc{};
  int gWidth{};
  int gHeight{};
  int wWidth{};
  int wHeight{};
};
