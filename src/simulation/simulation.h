#pragma once
#include "grid.h"

class Simulation
{
  public:
  Simulation() = default;

  bool init( uint16_t maxEnergy, uint16_t maxGenome, int width, int height, bool useHVDirections );
  void update();
  void pause();
  void resume();
  void reset();

  inline bool isPaused() const { return m_paused; }
  inline Grid& getGrid() { return m_grid; }
  inline const Grid& getGrid() const { return m_grid; }

  private:
  Grid m_grid;
  bool m_paused{ false };

  uint16_t m_maxEnergy;
  uint16_t m_maxGenome;
  int m_width;
  int m_height;
  bool m_useHVDirections;
};
