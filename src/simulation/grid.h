#pragma once
#include "cell.h"
#include "cell_factory.h"
#include "core/config.h"
#include <vector>
#include <cstdint>

class Grid
{
  public:
  Grid() = default;

  bool init( uint16_t maxEnergy, uint16_t maxGenome, int width, int height, bool useHVDirections );
  void update();
  void updatePixelBuffer();

  inline int getWidth() const { return m_width; }
  inline int getHeight() const { return m_height; }
  inline const std::vector<uint32_t>& getPixels() const { return m_pixels; }
  inline uint64_t getEpoch() const { return m_epoch; }

  Cell& getCell( int x, int y );
  const Cell& getCell( int x, int y ) const;

  Genome& getGenome( uint32_t index );
  const Genome& getGenome( uint32_t index ) const;

  private:
  std::vector<Cell> m_cells;
  std::vector<Genome> m_genomes;
  std::vector<uint32_t> m_pixels;

  CellFactory m_cellFactory{ Config::MAX_ENERGY, Config::MAX_GENOME, true };

  int m_width{ 0 };
  int m_height{ 0 };
  uint64_t m_epoch{ 0 };

  bool m_useHVDirections{ true };

  // Direction vectors
  static constexpr int DX8[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
  static constexpr int DY8[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
  static constexpr int DX4[] = { 0, 1, 0, -1 };
  static constexpr int DY4[] = { 1, 0, -1, 0 };

  inline int getIndex( int x, int y ) const { return y * m_width + x; }
  inline bool isInBounds( int x, int y ) const { return x >= 0 && x < m_width && y >= 0 && y < m_height; }

  void updateWood( Cell& cell, int x, int y );
  void updateLeaf( Cell& cell, int x, int y );
  void updateRoot( Cell& cell, int x, int y );
  void updateSprout( Cell& cell, int x, int y );

  uint32_t allocateGenome( Genome&& genome );
};
