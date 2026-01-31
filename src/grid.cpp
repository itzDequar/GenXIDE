#include <cstdlib>
#include <grid.h>

bool Grid::init( unsigned short maxEnergy, unsigned short maxGenome, int width, int height, bool isHV )
{
  bool success{ true };

  const char DX[] { 0, 1, 1, 1, 0, -1, -1, -1 };
  const char DY[] { 1, 1, 0, -1, -1, -1, 0, 1 };

  char HVDX[] { 0, 1, 0, -1, };
  char HVDY[] { 1, 0, -1, 0, };

  rnd.seed( std::random_device{}() );

  this->isHV = isHV;
  this->maxEnergy = maxEnergy;
  this->maxGenome = maxGenome;
  this->width = width;
  this->height = height;

  grid.resize(width*height);
  pixels.resize(width*height);

  for ( Cell& c : grid )
  {
    c = makeSprout();
  }
  updatePixels();

  return success;
}

void Grid::updatePixels()
{
  for ( int i = 0; i < (width*height); i++ )
  {
    pixels[i] = (0xFF << 24) | (grid[i].b << 16) | (grid[i].g << 8) | grid[i].r;
  }
}

void Grid::update()
{
  for (Cell& c : grid)
  {
    switch ( c.T )
    {
      case TYPE_EMPTY: continue; break;
      case TYPE_WOOD: upWood( &c ); break;
      case TYPE_LEAF: upLeaf( &c ); break;
      case TYPE_ROOT: upRoot( &c ); break;
      case TYPE_SPROUT: upSprout( &c ); break;
    }

  }
  updatePixels();
  epoch++;
}

void Grid::upWood( Cell* c )
{

}
void Grid::upLeaf( Cell* c )
{

}
void Grid::upRoot( Cell* c )
{

}
void Grid::upSprout( Cell* c )
{

}

Cell Grid::makeEmpty()
{
  Cell c{};

  c.T = TYPE_EMPTY;
  c.r = 0;
  c.g = 0;
  c.b = 0;

  return c;
}
Cell Grid::makeWood()
{
  Cell c{};

  c.T = TYPE_WOOD;
  c.energy = 0;
  if ( isHV )
  {
    std::uniform_int_distribution<unsigned char> d(0, 3);
    c.dir = d(rnd);
  } else
  {
    std::uniform_int_distribution<unsigned char> d(0, 7);
    c.dir = d(rnd);
  }
  c.r = 51;
  c.g = 51;
  c.b = 51;

  return c;
}
Cell Grid::makeLeaf()
{
  Cell c{};

  c.T = TYPE_LEAF;
  c.energy = 0;
  if ( isHV )
  {
    std::uniform_int_distribution<unsigned char> d(0, 3);
    c.dir = d(rnd);
  } else
  {
    std::uniform_int_distribution<unsigned char> d(0, 7);
    c.dir = d(rnd);
  }
  c.r = 12;
  c.g = 255;
  c.b = 51;

  return c;
}
Cell Grid::makeRoot()
{
  Cell c{};

  c.T = TYPE_ROOT;
  c.energy = 0;
  if ( isHV )
  {
    std::uniform_int_distribution<unsigned char> d(0, 3);
    c.dir = d(rnd);
  } else
  {
    std::uniform_int_distribution<unsigned char> d(0, 7);
    c.dir = d(rnd);
  }
  c.r = 16;
  c.g = 85;
  c.b = 2;

  return c;
}
Cell Grid::makeSprout()
{
  Cell c{};

  c.T = TYPE_SPROUT;
  c.energy = 0;
  if ( isHV )
  {
    std::uniform_int_distribution<unsigned char> d(0, 3);
    c.dir = d(rnd);
  } else
  {
    std::uniform_int_distribution<unsigned char> d(0, 7);
    c.dir = d(rnd);
  }
  c.genome.resize(maxGenome);
  for ( unsigned short& gen : c.genome )
  {
    std::uniform_int_distribution<unsigned char> d(0, maxGenome - 1);
    gen = d(rnd);
  }
  // c.r = 255;
  // c.g = 255;
  // c.b = 255;
  c.r = (uint8_t)( (float)c.genome[0] / (maxGenome - 1) * 255.0f );
  c.g = (uint8_t)( (float)c.genome[1] / (maxGenome - 1) * 255.0f );
  c.b = (uint8_t)( (float)c.genome[2] / (maxGenome - 1) * 255.0f );

  return c;
}


int Grid::getIdx( int x, int y ) { return x+y*width; }
bool Grid::isIn( int x, int y ) { return ( ( x >= 0 && x < width ) && ( y >= 0 && y < height ) ); }
