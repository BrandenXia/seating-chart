cmake_minimum_required(VERSION 3.16)
project(seating-chart)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wno-braced-scalar-init")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(seating-chart src/main.cpp src/seating_chart.cpp)
