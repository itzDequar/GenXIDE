#pragma once
#include <random>
#include <vector>

enum
{
  TYPE_EMPTY = 0,
  TYPE_WOOD,
  TYPE_LEAF,
  TYPE_ROOT,
  TYPE_SPROUT,
};
struct Cell
{
  unsigned char T{};
  unsigned short energy{};
  unsigned char dir{};
  uint8_t r, g, b;

  std::vector<unsigned short> genome{};
};

class Grid
{
  public:
  bool init( unsigned short maxEnergy, unsigned short maxGenome, int width, int height, bool isHV );

  int getWidth() { return width; };
  int getHeight() { return height; }

  private:
  std::vector<Cell> grid{};

  unsigned int epoch{ 0 };

  bool isHV{};

  unsigned short maxEnergy{};
  unsigned short maxGenome{};

  int width{};
  int height{};

  std::mt19937 rnd{};

  void update();

  void upWood( Cell* c );
  void upLeaf( Cell* c );
  void upRoot( Cell* c );
  void upSprout( Cell* c );

  Cell makeEmpty();
  Cell makeWood();
  Cell makeLeaf();
  Cell makeRoot();
  Cell makeSprout();

  int getIdx( int x, int y );
  bool isIn( int x, int y );
};
