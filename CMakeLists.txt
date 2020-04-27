cmake_minimum_required(VERSION 3.13)  # CMake version check
project(roulette)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")


# Add main.c file of project root directory as source file
set(SOURCE_FILES roulette.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(roulette ${SOURCE_FILES})
