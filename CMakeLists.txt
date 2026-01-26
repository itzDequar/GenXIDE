cmake_minimum_required(VERSION 3.14)
project(GenaXIDE C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/SDL)

include_directories(lib/glad/include/glad)
include_directories(lib/glad/include/KHR)
include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(src)

set(LIB_SOURCES
  lib/imgui/imgui.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/imgui_demo.cpp
  lib/imgui/backends/imgui_impl_sdl3.cpp
  lib/imgui/backends/imgui_impl_opengl3.cpp

  lib/glad/src/glad.c
)

add_executable(${PROJECT_NAME}
  src/source.cpp

  ${LIB_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 m)
