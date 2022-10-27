cmake_minimum_required(VERSION 3.11)

project(GuitarHero)

add_executable(${PROJECT_NAME} main.cpp window.cpp player.cpp platforms.cpp)

enable_abcg(${PROJECT_NAME})
