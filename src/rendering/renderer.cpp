#include "renderer.h"
#include <iostream>

Renderer::~Renderer()
{
  destroy();
}

bool Renderer::init( int gridWidth, int gridHeight )
{
  m_gridWidth = gridWidth;
  m_gridHeight = gridHeight;

  // Load shader
  if ( !m_shader.loadFromFiles("shaders/grid.vert", "shaders/grid.frag") )
  {
    std::cerr << "Failed to load shader" << std::endl;
    return false;
  }

  // Create grid texture
  if ( !m_gridTexture.create(gridWidth, gridHeight, GL_RGBA) )
  {
    std::cerr << "Failed to create grid texture" << std::endl;
    return false;
  }

  // Initialize camera at grid center
  m_camera = Camera2D(
    gridWidth * 0.5f,
    gridHeight * 0.5f,
    2.0f / gridHeight
  );

  // Create quad geometry
  createQuadGeometry();

  return true;
}

void Renderer::destroy()
{
  if ( m_vao != 0 )
  {
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
  }

  if ( m_vbo != 0 )
  {
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
  }

  if ( m_ebo != 0 )
  {
    glDeleteBuffers(1, &m_ebo);
    m_ebo = 0;
  }

  m_gridTexture.destroy();
  m_shader.destroy();
}

void Renderer::render( const Grid& grid, int windowWidth, int windowHeight )
{
  // Update texture with grid pixels
  m_gridTexture.update(grid.getPixels());

  // Clear screen
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Calculate aspect ratio
  float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

  // Get view matrix
  float viewMatrix[16];
  m_camera.getViewMatrix(viewMatrix, aspect);

  // Use shader and set uniforms
  m_shader.use();
  m_shader.setInt("uTex", 0);
  m_shader.setMat4("uMVP", viewMatrix);

  // Bind texture
  m_gridTexture.bind(0);

  // Draw quad
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
  glBindVertexArray(0);
}

void Renderer::handleResize( int windowWidth, int windowHeight )
{
  glViewport(0, 0, windowWidth, windowHeight);
}

void Renderer::createQuadGeometry()
{
  struct Vertex
  {
    float pos[2];
    float uv[2];
  };

  Vertex vertices[] = {
    {{0.0f, 0.0f}, {0.0f, 0.0f}},
    {{static_cast<float>(m_gridWidth), 0.0f}, {1.0f, 0.0f}},
    {{static_cast<float>(m_gridWidth), static_cast<float>(m_gridHeight)}, {1.0f, 1.0f}},
    {{0.0f, static_cast<float>(m_gridHeight)}, {0.0f, 1.0f}},
  };

  uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ebo);

  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // UV attribute
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));

  glBindVertexArray(0);
}
