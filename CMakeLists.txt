cmake_minimum_required(VERSION 3.1)
project(sway_display_switch)

set(CMAKE_CXX_STANDARD 20)

find_package(nlohmann_json REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(Fontconfig REQUIRED)

add_executable(sway_display_switch src/main.cpp
        src/Sway.cpp
        src/Sway.h
        src/ui/Output.cpp
        src/ui/Output.h
        src/ui/GUI.cpp
        src/ui/GUI.h
        src/ui/Icons.h
        src/ui/FontManager.cpp
        src/ui/FontManager.h)

target_link_libraries(sway_display_switch PRIVATE nlohmann_json::nlohmann_json SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image Fontconfig::Fontconfig)
