cmake_minimum_required(VERSION 3.17)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)

#make Game Library
add_library(Game Game/Game.cpp Game/aStar.cpp Game/Structures.cpp)
target_link_libraries(Game ncurses)

# Release executable
add_executable(Release main.cpp)
target_link_libraries(Release Game)

# Debug executable
add_executable(Debug Debug.cpp)
target_link_libraries(Debug Game)

# Experimental executable
add_executable(Experimental test.cpp)
target_link_libraries(Experimental Game)