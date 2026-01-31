#pragma once
#include "shader.h"
#include "texture.h"
#include "camera2d.h"
#include "../simulation/grid.h"
#include <glad/glad.h>
#include <vector>

class Renderer
{
  public:
  Renderer() = default;
  ~Renderer();

  bool init( int gridWidth, int gridHeight );
  void destroy();

  void render( const Grid& grid, int windowWidth, int windowHeight );
  void handleResize( int windowWidth, int windowHeight );

  inline Camera2D& getCamera() { return m_camera; }
  inline const Camera2D& getCamera() const { return m_camera; }

  private:
  Shader m_shader;
  Texture m_gridTexture;
  Camera2D m_camera;

  GLuint m_vao{ 0 };
  GLuint m_vbo{ 0 };
  GLuint m_ebo{ 0 };

  int m_gridWidth{ 0 };
  int m_gridHeight{ 0 };

  void createQuadGeometry();
};
