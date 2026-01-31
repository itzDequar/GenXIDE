#pragma once
#include <glad/glad.h>
#include <cstdint>
#include <vector>

class Texture
{
  public:
  Texture() = default;
  ~Texture();

  bool create( int width, int height, GLenum format = GL_RGBA );
  void destroy();

  void update( const void* data, int x, int y, int width, int height );
  void update( const std::vector<uint32_t>& pixels );

  void bind( GLuint textureUnit = 0 ) const;
  void unbind() const;

  inline GLuint getID() const { return m_textureID; }
  inline int getWidth() const { return m_width; }
  inline int getHeight() const { return m_height; }

  private:
  GLuint m_textureID{ 0 };
  int m_width{ 0 };
  int m_height{ 0 };
  GLenum m_format{ GL_RGBA };
};
