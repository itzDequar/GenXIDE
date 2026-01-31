#include "grid.h"
#include <algorithm>

bool Grid::init( uint16_t maxEnergy, uint16_t maxGenome, int width, int height, bool useHVDirections )
{
  m_width = width;
  m_height = height;
  m_useHVDirections = useHVDirections;

  m_cellFactory = CellFactory(maxEnergy, maxGenome, useHVDirections);

  const size_t totalCells = width * height;
  m_cells.resize(totalCells);
  m_pixels.resize(totalCells);

  // Reserve some genome space
  m_genomes.reserve(1024);

  // Initialize grid with sprouts
  for ( int y = 0; y < height; ++y )
  {
    for ( int x = 0; x < width; ++x )
    {
      Genome genome = m_cellFactory.createRandomGenome();
      uint32_t genomeIdx = allocateGenome(std::move(genome));

      Cell& cell = getCell(x, y);
      cell = m_cellFactory.createSprout(genomeIdx);

      // Set color based on first 3 genes
      const Genome& g = getGenome(genomeIdx);
      cell.r = static_cast<uint8_t>((float)g.genes[0] / (maxGenome - 1) * 255.0f);
      cell.g = static_cast<uint8_t>((float)g.genes[1] / (maxGenome - 1) * 255.0f);
      cell.b = static_cast<uint8_t>((float)g.genes[2] / (maxGenome - 1) * 255.0f);
    }
  }

  updatePixelBuffer();
  return true;
}

void Grid::update()
{
  // Update each cell based on its type
  for ( int y = 0; y < m_height; ++y )
  {
    for ( int x = 0; x < m_width; ++x )
    {
      Cell& cell = getCell(x, y);

      switch ( cell.type )
      {
        case CellType::Empty:
          break;
        case CellType::Wood:
          updateWood(cell, x, y);
          break;
        case CellType::Leaf:
          updateLeaf(cell, x, y);
          break;
        case CellType::Root:
          updateRoot(cell, x, y);
          break;
        case CellType::Sprout:
          updateSprout(cell, x, y);
          break;
      }

      if ( cell.isAlive() )
      {
        cell.age++;
      }
    }
  }

  updatePixelBuffer();
  m_epoch++;
}

void Grid::updatePixelBuffer()
{
  const size_t size = m_cells.size();
  for ( size_t i = 0; i < size; ++i )
  {
    m_pixels[i] = m_cells[i].toRGBA();
  }
}

Cell& Grid::getCell( int x, int y )
{
  return m_cells[getIndex(x, y)];
}

const Cell& Grid::getCell( int x, int y ) const
{
  return m_cells[getIndex(x, y)];
}

Genome& Grid::getGenome( uint32_t index )
{
  return m_genomes[index];
}

const Genome& Grid::getGenome( uint32_t index ) const
{
  return m_genomes[index];
}

uint32_t Grid::allocateGenome( Genome&& genome )
{
  uint32_t index = static_cast<uint32_t>(m_genomes.size());
  m_genomes.push_back(std::move(genome));
  return index;
}

void Grid::updateWood( Cell& cell, int x, int y )
{
  // TODO: Implement wood cell logic
}

void Grid::updateLeaf( Cell& cell, int x, int y )
{
  // TODO: Implement leaf cell logic
}

void Grid::updateRoot( Cell& cell, int x, int y )
{
  // TODO: Implement root cell logic
}

void Grid::updateSprout( Cell& cell, int x, int y )
{
  // TODO: Implement sprout cell logic
}
