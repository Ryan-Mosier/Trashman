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

set(BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(RENAME ${BIN_DIR}/Release ${BIN_DIR}/../Release)
file(RENAME ${BIN_DIR}/Debug ${BIN_DIR}/../Debug)
file(RENAME ${BIN_DIR}/Experimental ${BIN_DIR}/../Experimental)