#pragma once
#include <cstdint>

namespace Config
{
  // Window settings
  constexpr const char* WINDOW_TITLE = "GenXIDE";
  constexpr int WINDOW_WIDTH = 900;
  constexpr int WINDOW_HEIGHT = 600;

  // Grid settings
  constexpr int GRID_WIDTH = 256;
  constexpr int GRID_HEIGHT = 256;

  // Simulation settings
  constexpr uint16_t MAX_ENERGY = 100;
  constexpr uint16_t MAX_GENOME = 256;
  constexpr bool USE_HV_DIRECTIONS = true; // true = 4 directions, false = 8 directions

  // Rendering settings
  constexpr float INITIAL_ZOOM = 2.0f;
  constexpr float MIN_ZOOM = 0.0005f;
  constexpr float MAX_ZOOM = 20.0f;
  constexpr float ZOOM_SPEED = 1.15f;

  // Performance settings
  constexpr bool ENABLE_VSYNC = false;
  constexpr int TARGET_FPS = 60;
}
