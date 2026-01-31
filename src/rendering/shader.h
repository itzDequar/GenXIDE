#pragma once
#include <glad/glad.h>
#include <string>

class Shader
{
  public:
  Shader() = default;
  ~Shader();

  bool loadFromFiles( const char* vertexPath, const char* fragmentPath );
  bool loadFromSource( const char* vertexSource, const char* fragmentSource );
  void destroy();

  void use() const;
  inline GLuint getProgram() const { return m_program; }

  // Uniform setters
  void setInt( const char* name, int value ) const;
  void setFloat( const char* name, float value ) const;
  void setMat4( const char* name, const float* value ) const;

  GLint getUniformLocation( const char* name ) const;

  private:
  GLuint m_program{ 0 };

  GLuint compileShader( GLenum type, const char* source );
  bool linkProgram( GLuint vertexShader, GLuint fragmentShader );
};
