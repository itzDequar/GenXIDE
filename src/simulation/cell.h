#pragma once
#include <cstdint>
#include <vector>

enum class CellType : uint8_t
{
  Empty = 0,
  Wood,
  Leaf,
  Root,
  Sprout
};

// Optimized cell structure - 16 bytes without genome, genome stored separately
struct Cell
{
  CellType type{ CellType::Empty };
  uint8_t direction{ 0 };
  uint16_t energy{ 0 };
  
  uint8_t r{ 0 };
  uint8_t g{ 0 };
  uint8_t b{ 0 };
  uint8_t padding{ 0 }; // Alignment
  
  uint32_t genomeIndex{ 0 }; // Index to separate genome storage
  uint32_t age{ 0 };

  // Helper methods
  inline bool isEmpty() const { return type == CellType::Empty; }
  inline bool isAlive() const { return type != CellType::Empty; }
  inline uint32_t toRGBA() const { return (0xFF << 24) | (b << 16) | (g << 8) | r; }
};

// Separate genome storage for memory efficiency
struct Genome
{
  std::vector<uint16_t> genes;
  
  Genome() = default;
  explicit Genome( size_t size ) : genes(size) {}
};
