cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3 COMPONENTS Graphics Window System QUIET)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
endif()

add_executable(snake
    src/main.cpp
)

if(TARGET SFML::Graphics)
    target_link_libraries(snake PRIVATE SFML::Graphics SFML::Window SFML::System)
else()
    target_link_libraries(snake PRIVATE sfml-graphics sfml-window sfml-system)
endif()
