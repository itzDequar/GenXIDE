#include <iostream>
#include <renderer.h>
#include <fstream>
#include <sstream>
#include <iostream>

static std::string read_file(const char* path)
{
  std::ifstream file(path, std::ios::in);
  if (!file) {
    std::cerr << "Failed to open shader file: " << path << std::endl;
    return {};
  }

  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}
static GLuint compile_shader(GLenum type, const char* src)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint ok = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char log[1024];
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    std::cerr << "Shader compile error:\n" << log << std::endl;
  }
  return shader;
}
static GLuint load_shader_program( const char* vertex_path, const char* fragment_path )
{
  std::string vs_src = read_file(vertex_path);
  std::string fs_src = read_file(fragment_path);

  GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_src.c_str());
  GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_src.c_str());

  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  GLint ok = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &ok);
  if (!ok) {
    char log[1024];
    glGetProgramInfoLog(program, sizeof(log), nullptr, log);
    std::cerr << "Program link error:\n" << log << std::endl;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

bool Renderer::init( SDL_Window* window, int width, int height, int gridW, int gridH, std::vector<uint32_t>& pixels )
{
  bool success{ true };

  gWidth = gridW;
  gHeight = gridH;
  wWidth = width;
  wHeight = height;
  win = window;

  program = load_shader_program( "grid.vert", "grid.frag" );

  glGenTextures(1, &gridTex);
  glBindTexture(GL_TEXTURE_2D, gridTex);

  glTexImage2D
  (
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    gWidth,
    gHeight,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    nullptr
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, gridTex);
  glTexSubImage2D(
    GL_TEXTURE_2D,
    0,
    0, 0,
    gWidth,
    gHeight,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    pixels.data()
  );

  struct Vertex {
    float pos[2];
    float uv[2];
  };

  Vertex quad[] = {
    {{-1, -1}, {0, 0}},
    {{ 1, -1}, {1, 0}},
    {{ 1,  1}, {1, 1}},
    {{-1,  1}, {0, 1}},
  };

  uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };


  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(sizeof(float) * 2));

  glBindVertexArray(0);

  texLoc = glGetUniformLocation(program, "uTex");


  return success;
}

bool Renderer::destroy()
{
  glDeleteTextures(1, &gridTex);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
  glDeleteProgram(program);
  return true;
}

void Renderer::sizeChanged( SDL_Event event, SDL_Window* window )
{
  if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
  }
}

void Renderer::renderer( std::vector<uint32_t>& pixels )
{
  glBindTexture(GL_TEXTURE_2D, gridTex);
  glTexSubImage2D(
    GL_TEXTURE_2D,
    0,
    0, 0,
    gWidth,
    gHeight,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    pixels.data()
  );

  //glViewport(0, 0, wWidth, wHeight);
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(program);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gridTex);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  SDL_GL_SwapWindow(win);

  return;
}
