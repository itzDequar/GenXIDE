# Quick Reference Guide

## File Locations

### Need to change window size or grid size?
→ `src/core/config.h`

### Need to add a new cell type?
→ 1. `src/simulation/cell.h` (add enum)
→ 2. `src/simulation/cell_factory.cpp` (add factory method)
→ 3. `src/simulation/grid.cpp` (add update logic)

### Need to modify rendering?
→ `src/rendering/renderer.cpp`
→ `shaders/grid.vert` or `shaders/grid.frag`

### Need to change UI?
→ `src/ui/interface.cpp`

### Need to modify simulation logic?
→ `src/simulation/grid.cpp` (update methods)

### Need to change camera behavior?
→ `src/rendering/camera2d.cpp`

## Common Tasks

### Build the project
```bash
mkdir build && cd build
cmake ..
cmake --build .
./GenaXIDE
```

### Change grid size
Edit `src/core/config.h`:
```cpp
constexpr int GRID_WIDTH = 2048;  // Change here
constexpr int GRID_HEIGHT = 1024; // And here
```

### Add new cell type
1. Add to enum:
```cpp
enum class CellType : uint8_t {
  Empty, Wood, Leaf, Root, Sprout, YourNewType
};
```

2. Create factory method:
```cpp
Cell CellFactory::createYourNewType() {
  Cell c{};
  c.type = CellType::YourNewType;
  c.r = 255; c.g = 0; c.b = 0; // Set color
  return c;
}
```

3. Add update logic:
```cpp
case CellType::YourNewType:
  updateYourNewType(cell, x, y);
  break;
```

### Modify camera zoom speed
Edit `src/core/config.h`:
```cpp
constexpr float ZOOM_SPEED = 1.2f; // Faster zoom
```

### Add UI element
Edit `src/ui/interface.cpp` in `render()` method:
```cpp
ImGui::Text("Your text here");
ImGui::Button("Your button");
```

## Class Quick Reference

### Application
- `init()` - Initialize all systems
- `run()` - Main loop
- `shutdown()` - Clean up

### Window
- `isRunning()` - Check if window is open
- `getEvent()` - Get current SDL event
- `swapBuffers()` - Swap OpenGL buffers

### Simulation
- `update()` - Update one step
- `pause()` / `resume()` - Control simulation
- `getGrid()` - Access grid data

### Grid
- `getCell(x, y)` - Get cell at position
- `getWidth()` / `getHeight()` - Grid dimensions
- `getPixels()` - Get pixel buffer for rendering

### Renderer
- `render()` - Render grid
- `getCamera()` - Access camera

### Camera2D
- `zoomAt(x, y, factor)` - Zoom at world position
- `drag(x, y, aspect)` - Pan camera
- `getViewMatrix()` - Get transformation matrix

### Interface
- `render()` - Draw UI
- `wantCaptureMouse()` - Check if UI wants mouse

## Important Constants

```cpp
// In config.h
WINDOW_TITLE          // Window name
WINDOW_WIDTH          // Initial window width
WINDOW_HEIGHT         // Initial window height
GRID_WIDTH            // Grid width in cells
GRID_HEIGHT           // Grid height in cells
MAX_ENERGY            // Maximum cell energy
MAX_GENOME            // Genome size
USE_HV_DIRECTIONS     // 4 or 8 directions
INITIAL_ZOOM          // Starting zoom level
MIN_ZOOM / MAX_ZOOM   // Zoom limits
ZOOM_SPEED            // Zoom factor per scroll
ENABLE_VSYNC          // Vertical sync on/off
```

## Memory Layout

```
Grid owns:
  - vector<Cell> cells (contiguous)
  - vector<Genome> genomes (separate)
  - vector<uint32_t> pixels (for rendering)

Each Cell: 16 bytes
Each Genome: variable (vector of genes)
```

## Performance Tips

1. **Cells are contiguous** - iterating is cache-friendly
2. **Genomes separate** - only access when needed
3. **Inline methods** - zero overhead accessors
4. **Const correctness** - compiler optimizations
5. **RAII** - no manual memory management

## Debugging

- Enable ImGui demo: Check "Show Demo Window" in UI
- Check FPS in main window
- Camera position and zoom displayed in UI
- Epoch counter shows simulation progress

## Common Issues

**Shaders not found?**
→ Make sure `shaders/` folder is copied to build directory
→ CMake should do this automatically

**Grid not updating?**
→ Check if simulation is paused in UI

**Camera not moving?**
→ Make sure ImGui is not capturing mouse input

**Black screen?**
→ Check if shaders compiled successfully (console output)
