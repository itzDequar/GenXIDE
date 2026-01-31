# GenXIDE - Refactored Architecture

## Overview
GenXIDE Is IDE, for CyberBio

## Key Improvements

### 1. **Modular Architecture**
   - Clear separation of concerns
   - Each module has a specific responsibility
   - Easy to maintain and extend

### 2. **Memory Optimization**
   - Cell structure optimized to 16 bytes (without genome)
   - Genomes stored separately to reduce memory footprint
   - Contiguous memory layout for better cache performance

### 3. **Code Organization**
   - Single Responsibility Principle
   - Dependency Injection
   - Clean interfaces between modules

### 4. **Performance Enhancements**
   - Optimized pixel buffer updates
   - Efficient camera transformations
   - Reduced memory allocations

### 5. **Improved Resource Management**
   - RAII for all resources
   - Proper cleanup in destructors
   - No memory leaks

### 6. **Better Camera System**
   - Smooth zoom towards cursor
   - Improved pan and drag
   - Proper world-to-screen transformations

### 7. **Centralized Configuration**
   - All constants in Config namespace
   - Easy to adjust parameters
   - Compile-time configuration

## Class Responsibilities

### Core Module
- **Application**: Main application lifecycle, event loop
- **Window**: SDL window and OpenGL context management
- **Config**: Compile-time configuration constants

### Simulation Module
- **Cell**: Data structure for individual cells
- **CellFactory**: Factory pattern for creating different cell types
- **Grid**: Manages the 2D grid of cells
- **Simulation**: High-level simulation controller

### Rendering Module
- **Renderer**: Main rendering logic
- **Camera2D**: 2D camera with zoom and pan
- **Shader**: Shader compilation and management
- **Texture**: OpenGL texture wrapper

### UI Module
- **Interface**: ImGui integration and UI rendering

### Utils Module
- **FileUtils**: File reading utilities

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

The refactored code maintains the same functionality as the original but with:
- Cleaner code structure
- Better performance
- Easier to extend with new features
- More maintainable codebase

## Future Extension Points

The new architecture makes it easy to add:
1. **New Cell Types**: Add cases to CellFactory
2. **Save/Load System**: Implement in Simulation class
3. **Statistics**: Add to Interface class
4. **Performance Profiling**: Add to Application class
5. **Multithreading**: Grid update can be parallelized
6. **Custom Shaders**: Easy to swap in Renderer
7. **Different Rendering Modes**: Extend Renderer class

## Configuration

Edit `src/core/config.h` to adjust:
- Window size
- Grid dimensions
- Simulation parameters
- Rendering settings
- Performance options

## Code Style

The refactored code follows:
- Modern C++20 practices
- RAII for resource management
- Const correctness
- Clear naming conventions
- Minimal use of raw pointers
- Proper encapsulation
