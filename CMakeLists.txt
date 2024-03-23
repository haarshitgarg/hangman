cmake_minimum_required(VERSION 3.10)
project(hangman)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cc")

# Add executable
add_executable(hangman ${SOURCES})

