#include <iostream>
#include "renderer.h"
#include "window.h"
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
static void make_camera_matrix(
    float* m,
    float camX,
    float camY,
    float zoom,
    float aspect
) {
    // column-major (OpenGL)
    float sx = zoom / aspect;
    float sy = zoom;

    m[0]  = sx;  m[4]  = 0;   m[8]  = 0;  m[12] = -camX * sx;
    m[1]  = 0;   m[5]  = sy;  m[9]  = 0;  m[13] = -camY * sy;
    m[2]  = 0;   m[6]  = 0;   m[10] = 1;  m[14] = 0;
    m[3]  = 0;   m[7]  = 0;   m[11] = 0;  m[15] = 1;
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
  texLoc = glGetUniformLocation(program, "uTex");
  uMvpLoc = glGetUniformLocation(program, "uMVP");

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
    {{0, 0}, {0, 0}},
    {{(float)gWidth, 0}, {1, 0}},
    {{(float)gWidth, (float)gHeight}, {1, 1}},
    {{0, (float)gHeight}, {0, 1}},
  };

  uint16_t indices[] = { 0, 1, 2, 0, 2, 3 };

  cam.x = gWidth * 0.5f;
  cam.y = gHeight * 0.5f;
  cam.zoom = 2.0f / gHeight;


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

void Renderer::panZoom( Window& win )
{
  int fbW, fbH;
  SDL_GL_GetDrawableSize(win.getWindow(), &fbW, &fbH);
  const float speed = 2.0f / cam.zoom;
  if (win.getEvent().type == SDL_MOUSEWHEEL)
  {
    int mx, my;
    SDL_GetMouseState(&mx, &my);

    float zoomFactor = (win.getEvent().wheel.y > 0) ? 1.1f : 0.9f;

    float ndcX = (2.0f * mx / fbW) - 1.0f;
    float ndcY = 1.0f - (2.0f * my / fbH);

    float aspect = (float)fbW / fbH;

    float worldX = cam.x + ndcX / (cam.zoom / aspect);
    float worldY = cam.y + ndcY / cam.zoom;

    cam.zoom *= zoomFactor;
    cam.zoom = std::clamp(cam.zoom, 0.0005f, 10.0f);

    cam.x = worldX - ndcX / (cam.zoom / aspect);
    cam.y = worldY - ndcY / cam.zoom;
  }
  if (win.getEvent().type == SDL_MOUSEBUTTONDOWN && win.getEvent().button.button == SDL_BUTTON_MIDDLE)
  {
    cam.dragging = true;
    cam.lastMouseX = win.getEvent().button.x;
    cam.lastMouseY = win.getEvent().button.y;
  }
  if (win.getEvent().type == SDL_MOUSEBUTTONUP && win.getEvent().button.button == SDL_BUTTON_MIDDLE)
  {
    cam.dragging = false;
  }
  if (win.getEvent().type == SDL_MOUSEMOTION && cam.dragging)
  {
    int dx = win.getEvent().motion.xrel;
    int dy = win.getEvent().motion.yrel;

    float aspect = (float)fbW / fbH;

    float worldPerPixelX = 2.0f / (cam.zoom * fbW);
    float worldPerPixelY = 2.0f / (cam.zoom * fbH);

    cam.x -= dx * worldPerPixelX * aspect;
    cam.y += dy * worldPerPixelY;
  }
}

void Renderer::sizeChanged( SDL_Event event, SDL_Window* window )
{
  if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    SDL_GL_GetDrawableSize(window, &wWidth, &wHeight);
    glViewport(0, 0, wWidth, wHeight);
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
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  float aspect = (float)wWidth / (float)wHeight;
  float worldPerPixelX = 2.0f / (cam.zoom * wWidth);
  float worldPerPixelY = 2.0f / (cam.zoom * wHeight);


  float mvp[16];

  make_camera_matrix(
    mvp,
    cam.x,
    cam.y,
    cam.zoom,
    aspect
  );

  glUseProgram(program);
  glUniform1i(texLoc, 0);
  glUniformMatrix4fv(uMvpLoc, 1, GL_FALSE, mvp);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gridTex);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  SDL_GL_SwapWindow(win);

  return;
}
