#pragma once
#include "cell.h"
#include <random>

class CellFactory
{
  public:
  CellFactory( uint16_t maxEnergy, uint16_t maxGenome, bool useHVDirections );

  Cell createEmpty();
  Cell createWood();
  Cell createLeaf();
  Cell createRoot();
  Cell createSprout( uint32_t genomeIndex );

  Genome createRandomGenome();

  inline uint16_t getMaxEnergy() const { return m_maxEnergy; }
  inline uint16_t getMaxGenome() const { return m_maxGenome; }

  private:
  uint16_t m_maxEnergy;
  uint16_t m_maxGenome;
  bool m_useHVDirections;
  std::mt19937 m_rng;

  uint8_t randomDirection();
};
