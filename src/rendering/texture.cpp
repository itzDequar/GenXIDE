#include "texture.h"

Texture::~Texture()
{
  destroy();
}

bool Texture::create( int width, int height, GLenum format )
{
  m_width = width;
  m_height = height;
  m_format = format;

  glGenTextures(1, &m_textureID);
  glBindTexture(GL_TEXTURE_2D, m_textureID);

  GLenum internalFormat = (format == GL_RGBA) ? GL_RGBA8 : GL_RGB8;

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    internalFormat,
    width,
    height,
    0,
    format,
    GL_UNSIGNED_BYTE,
    nullptr
  );

  // Nearest filtering for pixel-perfect rendering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

void Texture::destroy()
{
  if ( m_textureID != 0 )
  {
    glDeleteTextures(1, &m_textureID);
    m_textureID = 0;
  }
}

void Texture::update( const void* data, int x, int y, int width, int height )
{
  glBindTexture(GL_TEXTURE_2D, m_textureID);
  glTexSubImage2D(
    GL_TEXTURE_2D,
    0,
    x, y,
    width, height,
    m_format,
    GL_UNSIGNED_BYTE,
    data
  );
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::update( const std::vector<uint32_t>& pixels )
{
  update(pixels.data(), 0, 0, m_width, m_height);
}

void Texture::bind( GLuint textureUnit ) const
{
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
