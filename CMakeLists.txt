cmake_minimum_required(VERSION 3.14)
project(GenaXIDE C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/SDL2)

include_directories(lib/SDL2/include)
include_directories(lib/glad/include)
include_directories(lib/imgui)
include_directories(src)

set(LIB_SOURCES
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp

  lib/glad/src/glad.c
)

add_executable(${PROJECT_NAME}
  src/source.cpp
  src/window.cpp
  src/renderer.cpp
  src/grid.cpp

  ${LIB_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL2::SDL2
)

# for MacOS
if(APPLE)
  target_link_libraries(GenaXIDE PRIVATE "-framework Metal" "-framework Cocoa" "-framework QuartzCore")
endif()

# for Linux:
if(UNIX AND NOT APPLE)
  target_link_libraries(GenaXIDE PRIVATE GL X11 dl pthread)
endif()

# for Windows:
if(WIN32)
  target_link_libraries(GenaXIDE PRIVATE gdi32 user32 psapi)
endif()
