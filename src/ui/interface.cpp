#include "interface.h"
#include "../simulation/simulation.h"
#include "../rendering/camera2d.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

Interface::~Interface()
{
  destroy();
}

bool Interface::init( SDL_Window* window, SDL_GLContext glContext )
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_None;

  ImGui::StyleColorsClassic();

  ImGui_ImplSDL2_InitForOpenGL(window, glContext);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  return true;
}

void Interface::destroy()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void Interface::processEvent( const SDL_Event& event )
{
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void Interface::newFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  ImGuiIO& io = ImGui::GetIO();
  m_wantCaptureMouse = io.WantCaptureMouse;
  m_wantCaptureKeyboard = io.WantCaptureKeyboard;
}

void Interface::render( Simulation& simulation, Camera2D& camera )
{
  const Grid& grid = simulation.getGrid();

  ImGui::Begin("GenXIDE");

  // FPS counter
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

  // Grid info
  ImGui::Separator();
  ImGui::Text("Grid: %d x %d", grid.getWidth(), grid.getHeight());
  ImGui::Text("Epoch: %llu", static_cast<unsigned long long>(grid.getEpoch()));

  // Simulation controls
  ImGui::Separator();
  if ( simulation.isPaused() )
  {
    if ( ImGui::Button("Resume Simulation") )
    {
      simulation.resume();
    }
  }
  else
  {
    if ( ImGui::Button("Pause Simulation") )
    {
      simulation.pause();
    }
  }

  ImGui::SameLine();
  if ( ImGui::Button("Reset") )
  {
    simulation.reset();
  }

  // Camera info
  ImGui::Separator();
  ImGui::Text("Camera Position: (%.1f, %.1f)", camera.getX(), camera.getY());
  ImGui::Text("Camera Zoom: %.4f", camera.getZoom());

  // Debug window toggle
  ImGui::Separator();
  ImGui::Checkbox("Show Demo Window", &m_showDemoWindow);

  ImGui::End();

  // ImGui demo window
  // if ( m_showDemoWindow )
  // {
  //   ImGui::ShowDemoWindow(&m_showDemoWindow);
  // }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
