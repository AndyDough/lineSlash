cmake_minimum_required(VERSION 3.16)
project(lineSlash LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SFML 3 COMPONENTS Graphics REQUIRED)

add_executable(lineSlash
    src/main.cpp
)

target_link_libraries(lineSlash PRIVATE SFML::Graphics)
