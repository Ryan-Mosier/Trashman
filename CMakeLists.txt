cmake_minimum_required(VERSION 3.22)
project(Trashman)

set(CMAKE_CXX_STANDARD 17)

add_executable(Trashman main.cpp Level/Map.cpp Level/Map.h Level/colors.h)
