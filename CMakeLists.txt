cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point CMake to Homebrew’s SFML install
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/sfml/3.0.2")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(snake src/main.cpp)

target_link_libraries(snake PRIVATE SFML::Graphics SFML::Window SFML::System)
