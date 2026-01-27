cmake_minimum_required(VERSION 3.14)
project(GenaXIDE C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/SDL)
add_subdirectory(lib/cimgui)

include_directories(lib/glad/include/glad)
include_directories(lib/glad/include/KHR)
include_directories(lib/cimgui)
#include_directories(lib/imgui/backends)
include_directories(src)

set(LIB_SOURCES
  lib/glad/src/glad.c
)

add_executable(${PROJECT_NAME}
  src/source.c
  src/window.c

  ${LIB_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 m)
