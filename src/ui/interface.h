#pragma once
#include <SDL.h>
#include <glad/glad.h>

class Simulation;
class Camera2D;

class Interface
{
  public:
  Interface() = default;
  ~Interface();

  bool init( SDL_Window* window, SDL_GLContext glContext );
  void destroy();

  void processEvent( const SDL_Event& event );
  void newFrame();
  void render( Simulation& simulation, Camera2D& camera );

  inline bool wantCaptureMouse() const { return m_wantCaptureMouse; }
  inline bool wantCaptureKeyboard() const { return m_wantCaptureKeyboard; }

  private:
  bool m_wantCaptureMouse{ false };
  bool m_wantCaptureKeyboard{ false };
  bool m_showDemoWindow{ false };
};
