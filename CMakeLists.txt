cmake_minimum_required(VERSION 3.17)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyGame main.cpp Game/Game.cpp Game/Pathfinding.cpp)

add_executable(Test test.cpp)


target_link_libraries(MyGame ncurses)
target_link_libraries(Test ncurses)
