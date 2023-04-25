cmake_minimum_required(VERSION 3.17)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(GLFW)
add_subdirectory(rendering)


#make GameHandler Library
add_library(Game Game/Game.cpp Game/aStar.cpp Game/Structures.cpp Game/object.cpp Game/debugValues.cpp Game/objectHashTable.cpp)
add_library(Glad rendering/glad.c)

# Create a subdirectory in the root directory for the executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# Release executable

#add_executable(Release main.cpp)
#target_link_libraries(Release Game)

# Debug executable
#add_executable(Debug Debug.cpp)
#target_link_libraries(Debug Game)

# Experimental executable
add_executable(Release main.cpp)
target_link_libraries(Release renderer glfw Glad stb)