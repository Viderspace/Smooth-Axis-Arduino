cmake_minimum_required(VERSION 3.21)
project(SmoothAxisArduino)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/core)

add_executable(SmoothAxisArduino
        src/core/smooth_axis.c
        src/core/smooth_axis.h
        src/core/smooth_axis_debug.h
        src/SmoothAxis.h)
