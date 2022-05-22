cmake_minimum_required(VERSION 3.22)

project(emu)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(emu main.cpp Chip8.cpp Chip8.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
