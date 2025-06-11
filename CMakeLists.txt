cmake_minimum_required(VERSION 3.31)
project(sway_monitor_switch)

set(CMAKE_CXX_STANDARD 20)

add_executable(sway_monitor_switch src/main.cpp
        src/Sway.cpp
        src/Sway.h)
