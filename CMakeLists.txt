cmake_minimum_required(VERSION 3.14)
project(GenaXIDE C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/SDL)
add_subdirectory(libs/bgfx)

include_directories(src)

set(LIB_SOURCES
)

add_executable(${PROJECT_NAME}
  src/source.cpp
  src/window.cpp
  src/renderer.cpp

  ${LIB_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL3::SDL3
  bgfx::bgfx       # Сама графика
  bgfx::bimg       # Работа с картинками (нужна для текстур)
  bgfx::bx         # Базовые утилиты
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
