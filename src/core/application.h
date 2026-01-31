#pragma once
#include "window.h"
#include "../rendering/renderer.h"
#include "../simulation/simulation.h"
#include "../ui/interface.h"
#include "config.h"

class Application
{
  public:
  Application() = default;
  ~Application();

  bool init();
  void run();
  void shutdown();

  private:
  Window m_window;
  Renderer m_renderer;
  Simulation m_simulation;
  Interface m_interface;

  bool m_isRunning{ false };

  void processEvents();
  void update();
  void render();

  void handleCameraInput( const SDL_Event& event );
};
