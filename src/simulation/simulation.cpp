#include "simulation.h"

bool Simulation::init( uint16_t maxEnergy, uint16_t maxGenome, int width, int height, bool useHVDirections )
{
  m_maxEnergy = maxEnergy;
  m_maxGenome = maxGenome;
  m_width = width;
  m_height = height;
  m_useHVDirections = useHVDirections;

  return m_grid.init(maxEnergy, maxGenome, width, height, useHVDirections);
}

void Simulation::update()
{
  if ( !m_paused )
  {
    m_grid.update();
  }
}

void Simulation::pause()
{
  m_paused = true;
}

void Simulation::resume()
{
  m_paused = false;
}

void Simulation::reset()
{
  m_grid.init(m_maxEnergy, m_maxGenome, m_width, m_height, m_useHVDirections);
  m_paused = false;
}
