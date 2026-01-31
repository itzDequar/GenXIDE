# GenXIDE Architecture Diagram

## Module Dependencies

```
┌─────────────────────────────────────────────────────────────┐
│                         Application                          │
│                     (Core/application.cpp)                   │
│                   - Main loop orchestration                  │
│                   - Event processing                         │
└───────┬─────────────┬────────────┬──────────────┬───────────┘
        │             │            │              │
        ▼             ▼            ▼              ▼
   ┌────────┐   ┌──────────┐  ┌──────────┐  ┌──────────┐
   │ Window │   │Simulation│  │ Renderer │  │Interface │
   └────┬───┘   └─────┬────┘  └─────┬────┘  └─────┬────┘
        │             │              │             │
        │             │              │             │
        │         ┌───▼────┐    ┌────▼────┐       │
        │         │  Grid  │    │ Camera  │       │
        │         └───┬────┘    │  2D     │       │
        │             │         └────┬────┘       │
        │         ┌───▼──────┐  ┌────▼────┐       │
        │         │   Cell   │  │ Shader  │       │
        │         │ Factory  │  │ Texture │       │
        │         └──────────┘  └─────────┘       │
        │                                          │
        └──────────────────────────────────────────┘
                      SDL2 + OpenGL
```

## Data Flow

```
User Input
    │
    ├──> Window (SDL Events)
    │       │
    │       ├──> Interface (ImGui)
    │       │       │
    │       │       └──> Simulation Control (pause/resume/reset)
    │       │
    │       └──> Camera2D (pan/zoom)
    │
    ▼
Application Update Loop
    │
    ├──> Simulation::update()
    │       │
    │       └──> Grid::update()
    │               │
    │               ├──> For each Cell: updateWood/Leaf/Root/Sprout()
    │               │
    │               └──> Grid::updatePixelBuffer()
    │                       │
    │                       └──> pixels[] = cells[].toRGBA()
    │
    └──> Renderer::render()
            │
            ├──> Texture::update(pixels)
            │
            ├──> Camera2D::getViewMatrix()
            │
            ├──> Shader::use()
            │
            └──> Draw quad with grid texture
```

## Class Hierarchy

```
Application
  ├── Window (SDL window + GL context)
  ├── Simulation
  │   └── Grid
  │       ├── vector<Cell>
  │       ├── vector<Genome>
  │       ├── vector<uint32_t> pixels
  │       └── CellFactory
  ├── Renderer
  │   ├── Camera2D
  │   ├── Shader
  │   └── Texture
  └── Interface (ImGui)
```

## Memory Layout

```
Grid:
┌─────────────────────────────────────────┐
│ Cells (contiguous array)                │
│ [Cell][Cell][Cell][Cell]...[Cell]       │  <- 16 bytes each
│                                          │
│ Genomes (separate storage)              │
│ [Genome*][Genome*]...[Genome*]           │  <- Only when needed
│                                          │
│ Pixels (RGBA buffer)                    │
│ [RGBA][RGBA][RGBA]...[RGBA]             │  <- 4 bytes each
└─────────────────────────────────────────┘

Cell:
┌─────────────────┐
│ type      (1b)  │
│ direction (1b)  │
│ energy    (2b)  │
│ r,g,b     (3b)  │
│ padding   (1b)  │
│ genomeIdx (4b)  │
│ age       (4b)  │
└─────────────────┘
Total: 16 bytes
```

## Render Pipeline

```
Grid Pixels (CPU)
      ↓
Texture::update()
      ↓
GPU Texture (VRAM)
      ↓
Vertex Shader (grid.vert)
  - Apply MVP matrix
  - Pass UVs
      ↓
Fragment Shader (grid.frag)
  - Sample texture
  - Output color
      ↓
Screen Framebuffer
```

## Component Responsibilities

### Core Module
```
Application   → Lifecycle, main loop, coordination
Window        → SDL/OpenGL initialization, events
Config        → Compile-time constants
```

### Simulation Module
```
Cell          → Data structure (16 bytes)
Genome        → Gene storage (separate)
CellFactory   → Create cells with random genes
Grid          → 2D array of cells, update logic
Simulation    → High-level control (pause/resume)
```

### Rendering Module
```
Renderer      → Main render logic
Camera2D      → View transformation, pan/zoom
Shader        → GLSL program management
Texture       → OpenGL texture wrapper
```

### UI Module
```
Interface     → ImGui windows and controls
```

### Utils Module
```
FileUtils     → Read shader files
```

## Key Patterns Used

1. **RAII** - Automatic resource management
2. **Factory Pattern** - CellFactory creates cells
3. **Separation of Concerns** - Each class has one job
4. **Dependency Injection** - Pass dependencies to constructors
5. **Data-Oriented Design** - Cells in contiguous array
6. **Encapsulation** - Private data, public interface

## Performance Hotspots

```
Grid::update()          ← Main simulation logic (parallelizable)
Grid::updatePixelBuffer() ← Convert cells to RGBA (cache-friendly)
Texture::update()       ← Upload to GPU (minimize calls)
```

## Extension Points

```
New Cell Type:
  1. Add to CellType enum
  2. Add CellFactory::createXXX()
  3. Add Grid::updateXXX()

New Feature:
  - Statistics    → Add to Interface
  - Save/Load     → Add to Simulation
  - New Rendering → Extend Renderer
  - Threading     → Parallelize Grid::update()
```
