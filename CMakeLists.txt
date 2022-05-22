cmake_minimum_required(VERSION 3.22)
project(emu)

set(CMAKE_CXX_STANDARD 23)

add_executable(emu main.cpp Chip8.cpp Chip8.h)
