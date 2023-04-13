cmake_minimum_required(VERSION 3.17)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(GLFW)
add_subdirectory(rendering)

#make Game Library
add_library(Game Game/Game.cpp Game/aStar.cpp Game/Structures.cpp)
add_library(Glad rendering/glad.c)
target_link_libraries(Game ncurses)

# Create a subdirectory in the root directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# Release executable
add_executable(Release main.cpp)
target_link_libraries(Release Game)

# Debug executable
add_executable(Debug Debug.cpp)
target_link_libraries(Debug Game)

# Experimental executable
add_executable(Experimental test.cpp rendering/renderer.cpp)
target_link_libraries(Experimental Game OpenGL)
target_link_libraries(Experimental glfw glfw3 GL X11 pthread Xrandr Xi dl Glad)