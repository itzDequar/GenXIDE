#include "shader.h"
#include "../utils/file_utils.h"
#include <iostream>

Shader::~Shader()
{
  destroy();
}

bool Shader::loadFromFiles( const char* vertexPath, const char* fragmentPath )
{
  std::string vertexSource = FileUtils::readTextFile(vertexPath);
  std::string fragmentSource = FileUtils::readTextFile(fragmentPath);

  if ( vertexSource.empty() || fragmentSource.empty() )
  {
    return false;
  }

  return loadFromSource(vertexSource.c_str(), fragmentSource.c_str());
}

bool Shader::loadFromSource( const char* vertexSource, const char* fragmentSource )
{
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
  if ( vertexShader == 0 ) return false;

  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
  if ( fragmentShader == 0 )
  {
    glDeleteShader(vertexShader);
    return false;
  }

  bool success = linkProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return success;
}

void Shader::destroy()
{
  if ( m_program != 0 )
  {
    glDeleteProgram(m_program);
    m_program = 0;
  }
}

void Shader::use() const
{
  glUseProgram(m_program);
}

void Shader::setInt( const char* name, int value ) const
{
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat( const char* name, float value ) const
{
  glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat4( const char* name, const float* value ) const
{
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value);
}

GLint Shader::getUniformLocation( const char* name ) const
{
  return glGetUniformLocation(m_program, name);
}

GLuint Shader::compileShader( GLenum type, const char* source )
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if ( !success )
  {
    char infoLog[1024];
    glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
    std::cerr << "Shader compilation error (" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "):\n" << infoLog << std::endl;
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

bool Shader::linkProgram( GLuint vertexShader, GLuint fragmentShader )
{
  m_program = glCreateProgram();
  glAttachShader(m_program, vertexShader);
  glAttachShader(m_program, fragmentShader);
  glLinkProgram(m_program);

  GLint success = 0;
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);

  if ( !success )
  {
    char infoLog[1024];
    glGetProgramInfoLog(m_program, sizeof(infoLog), nullptr, infoLog);
    std::cerr << "Shader linking error:\n" << infoLog << std::endl;
    glDeleteProgram(m_program);
    m_program = 0;
    return false;
  }

  return true;
}
