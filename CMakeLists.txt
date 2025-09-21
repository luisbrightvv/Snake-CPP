cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# Find SFML (must be installed on the system)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

add_executable(snake src/main.cpp)

target_link_libraries(snake PRIVATE SFML::Graphics SFML::Window SFML::System)
