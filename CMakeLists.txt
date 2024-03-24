cmake_minimum_required(VERSION 3.10)
project(hangman)

set(CMAKE_EXPORT_COMPILE_COMMANDS=1)
# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#setting sfml include path
set(SFML_INCLUDE_PATH /opt/homebrew/Cellar/sfml/2.5.1_2/include)
# Include directories
include_directories(include)
include_directories(/opt/homebrew/Cellar/sfml/2.5.1_2/include)

# Source files
file(GLOB SOURCES "src/*.cc")
file(GLOB TEST_SOURCES "tests/*.cc")

# Add executable
add_executable(hangman ${SOURCES})
add_executable(hangman_test ${TEST_SOURCES})

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(hangman sfml-graphics sfml-window sfml-system)
target_link_libraries(hangman_test sfml-graphics sfml-window sfml-system)
