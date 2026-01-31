#include "cell_factory.h"

CellFactory::CellFactory( uint16_t maxEnergy, uint16_t maxGenome, bool useHVDirections )
  : m_maxEnergy(maxEnergy)
  , m_maxGenome(maxGenome)
  , m_useHVDirections(useHVDirections)
{
  m_rng.seed(std::random_device{}());
}

uint8_t CellFactory::randomDirection()
{
  if ( m_useHVDirections )
  {
    std::uniform_int_distribution<uint8_t> dist(0, 3);
    return dist(m_rng);
  }
  else
  {
    std::uniform_int_distribution<uint8_t> dist(0, 7);
    return dist(m_rng);
  }
}

Cell CellFactory::createEmpty()
{
  Cell c{};
  c.type = CellType::Empty;
  c.r = 0;
  c.g = 0;
  c.b = 0;
  return c;
}

Cell CellFactory::createWood()
{
  Cell c{};
  c.type = CellType::Wood;
  c.energy = 0;
  c.direction = randomDirection();
  c.r = 51;
  c.g = 51;
  c.b = 51;
  return c;
}

Cell CellFactory::createLeaf()
{
  Cell c{};
  c.type = CellType::Leaf;
  c.energy = 0;
  c.direction = randomDirection();
  c.r = 12;
  c.g = 255;
  c.b = 51;
  return c;
}

Cell CellFactory::createRoot()
{
  Cell c{};
  c.type = CellType::Root;
  c.energy = 0;
  c.direction = randomDirection();
  c.r = 16;
  c.g = 85;
  c.b = 2;
  return c;
}

Cell CellFactory::createSprout( uint32_t genomeIndex )
{
  Cell c{};
  c.type = CellType::Sprout;
  c.energy = 0;
  c.direction = randomDirection();
  c.genomeIndex = genomeIndex;
  
  // Color will be set based on genome
  c.r = 255;
  c.g = 255;
  c.b = 255;
  
  return c;
}

Genome CellFactory::createRandomGenome()
{
  Genome genome(m_maxGenome);
  std::uniform_int_distribution<uint16_t> dist(0, m_maxGenome - 1);
  
  for ( uint16_t& gene : genome.genes )
  {
    gene = dist(m_rng);
  }
  
  return genome;
}
