#include "application.h"
#include <iostream>

Application::~Application()
{
  shutdown();
}

bool Application::init()
{
  if ( !m_window.init(
    Config::WINDOW_TITLE,
    Config::WINDOW_WIDTH,
    Config::WINDOW_HEIGHT,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
    SDL_INIT_VIDEO | SDL_INIT_EVENTS
  ))
  {
    std::cerr << "Failed to initialize window" << std::endl;
    return false;
  }

  if ( !m_interface.init(m_window.getSDLWindow(), m_window.getGLContext()) )
  {
    std::cerr << "Failed to initialize interface" << std::endl;
    return false;
  }
  
  if ( !m_simulation.init(
    Config::MAX_ENERGY,
    Config::MAX_GENOME,
    Config::GRID_WIDTH,
    Config::GRID_HEIGHT,
    Config::USE_HV_DIRECTIONS
  ))
  {
    std::cerr << "Failed to initialize simulation" << std::endl;
    return false;
  }

  if ( !m_renderer.init(Config::GRID_WIDTH, Config::GRID_HEIGHT) )
  {
    std::cerr << "Failed to initialize renderer" << std::endl;
    return false;
  }

  m_isRunning = true;
  return true;
}

void Application::run()
{
  while ( m_isRunning && m_window.isRunning() )
  {
    processEvents();
    update();
    render();
  }
}

void Application::shutdown()
{
  m_renderer.destroy();
  m_interface.destroy();
  m_window.destroy();
}

void Application::processEvents()
{
  SDL_Event event;

  while ( SDL_PollEvent(&event) )
  {
    // Check for quit
    if ( event.type == SDL_QUIT )
    {
      m_isRunning = false;
    }

    // Process ImGui events first
    m_interface.processEvent(event);

    // Handle camera input (only if ImGui doesn't want to capture mouse)
    if ( !m_interface.wantCaptureMouse() )
    {
      handleCameraInput(event);
    }

    // Handle window resize
    if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
    {
      int width, height;
      m_window.getFramebufferSize(width, height);
      m_renderer.handleResize(width, height);
    }
  }
}

void Application::update()
{
  m_simulation.update();
}

void Application::render()
{
  int width, height;
  m_window.getFramebufferSize(width, height);

  // Render grid
  m_renderer.render(m_simulation.getGrid(), width, height);

  // Render UI
  m_interface.newFrame();
  m_interface.render(m_simulation, m_renderer.getCamera());

  // Swap buffers
  m_window.swapBuffers();
}

void Application::handleCameraInput( const SDL_Event& event )
{
  Camera2D& camera = m_renderer.getCamera();

  // Zoom with mouse wheel
  if ( event.type == SDL_MOUSEWHEEL )
  {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int fbWidth, fbHeight;
    m_window.getFramebufferSize(fbWidth, fbHeight);

    float aspect = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);

    // Calculate world position under mouse
    float ndcX = 1.0f - (2.0f * mouseX / fbWidth);
    float ndcY = (2.0f * mouseY / fbHeight) - 1.0f;

    float worldX = camera.getX() + ndcX / (camera.getZoom() / aspect);
    float worldY = camera.getY() + ndcY / camera.getZoom();

    // Zoom
    float zoomFactor = (event.wheel.y > 0) ? Config::ZOOM_SPEED : (1.0f / Config::ZOOM_SPEED);
    camera.zoomAt(worldX, worldY, zoomFactor);
  }

  // Pan with middle mouse button
  if ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_MIDDLE )
  {
    camera.startDrag(event.button.x, event.button.y);
  }

  if ( event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_MIDDLE )
  {
    camera.endDrag();
  }

  if ( event.type == SDL_MOUSEMOTION && camera.isDragging() )
  {
    int fbWidth, fbHeight;
    m_window.getFramebufferSize(fbWidth, fbHeight);
    float aspect = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);

    // Use relative motion (xrel, yrel)
    camera.drag(event.motion.xrel, event.motion.yrel, fbHeight);
  }
}
