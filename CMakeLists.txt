cmake_minimum_required(VERSION 3.1)
project(sway_monitor_switch)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json REQUIRED)

add_executable(sway_monitor_switch src/main.cpp
        src/Sway.cpp
        src/Sway.h)

target_link_libraries(sway_monitor_switch PRIVATE nlohmann_json::nlohmann_json)
