cmake_minimum_required(VERSION 3.10)
project(maze_game)
include_directories(inc)
add_executable(maze_game
    scr/main.cpp
    scr/point.cpp
    scr/map.cpp
    scr/player.cpp
)