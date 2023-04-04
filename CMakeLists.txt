cmake_minimum_required(VERSION 3.17)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)

#make Game Library
add_library(Gamel Game/Game.cpp Game/aStar.cpp Game/Structures.cpp)
target_link_libraries(Gamel ncurses)

# Release executable
add_executable(Release main.cpp)
target_link_libraries(Release Gamel)

# Debug executable
add_executable(Debug Debug.cpp)
target_link_libraries(Debug Gamel)

# Experimental executable
add_executable(Experimental test.cpp)
target_link_libraries(Experimental Gamel)