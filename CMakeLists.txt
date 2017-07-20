# please add this subdirectory to your project.
# and link with target_link_libraries(target taToolkit)

cmake_minimum_required(VERSION 2.6)
project(emily)
include_directories(include)
add_library(taToolkit STATIC 
# core
src/funcs.cpp
# SDL
src/sdl/funcs.cpp
)
target_link_libraries(taToolkit SDL2)
