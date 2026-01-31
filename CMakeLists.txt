cmake_minimum_required(VERSION 3.14)
project(GenaXIDE C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(MSVC)
    add_compile_options(/O2 /GL)
  else()
    add_compile_options(-O3 -march=native -flto)
  endif()
endif()

# Dependencies
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/SDL2)

# Include directories
include_directories(lib/SDL2/include)
include_directories(lib/glad/include)
include_directories(lib/imgui)
include_directories(src)

# ImGui sources
set(IMGUI_SOURCES
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/backends/imgui_impl_sdl2.cpp
  lib/imgui/backends/imgui_impl_sdl2.h
  lib/imgui/backends/imgui_impl_opengl3.cpp
  lib/imgui/backends/imgui_impl_opengl3.h
)

# GLAD
set(GLAD_SOURCES
  lib/glad/src/glad.c
)

# Core sources
set(CORE_SOURCES
  src/core/application.cpp
  src/core/application.h
  src/core/window.cpp
  src/core/window.h
  src/core/config.h
)

# Simulation sources
set(SIMULATION_SOURCES
  src/simulation/grid.cpp
  src/simulation/grid.h
  src/simulation/cell.h
  src/simulation/cell_factory.cpp
  src/simulation/cell_factory.h
  src/simulation/simulation.cpp
  src/simulation/simulation.h
)

# Rendering sources
set(RENDERING_SOURCES
  src/rendering/renderer.cpp
  src/rendering/renderer.h
  src/rendering/camera2d.cpp
  src/rendering/camera2d.h
  src/rendering/shader.cpp
  src/rendering/shader.h
  src/rendering/texture.cpp
  src/rendering/texture.h
)

# UI sources
set(UI_SOURCES
  src/ui/interface.cpp
  src/ui/interface.h
)

# Utils sources
set(UTILS_SOURCES
  src/utils/file_utils.cpp
  src/utils/file_utils.h
)

# Main executable
add_executable(${PROJECT_NAME}
  src/main.cpp
  ${CORE_SOURCES}
  ${SIMULATION_SOURCES}
  ${RENDERING_SOURCES}
  ${UI_SOURCES}
  ${UTILS_SOURCES}
  ${IMGUI_SOURCES}
  ${GLAD_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

# Platform-specific linking
if(APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Metal" "-framework Cocoa" "-framework QuartzCore")
endif()

if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE GL X11 dl pthread)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE gdi32 user32 psapi)
endif()

# Copy shaders to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
