# please add this subdirectory to your project.
# and link with target_link_libraries(target taToolkit)

cmake_minimum_required(VERSION 3.1)
project(emily)
set(CMAKE_CXX_STANDARD 11)
include_directories(include /usr/include/freetype2)
find_library(HAVE_SDL2 SDL2)
set(SOURCES src/funcs.cpp
            src/bitmap.cpp
            src/font.cpp
            
            src/widget/base.cpp
            src/widget/label.cpp
            
            src/dummy/funcs.cpp)
if (HAVE_SDL2)
  list(APPEND SOURCES src/sdl/funcs.cpp)
endif (HAVE_SDL2)
add_library(taToolkit STATIC ${SOURCES})
add_executable(hello examples/hello/hello.cpp)
if (HAVE_SDL2)
  target_compile_definitions(taToolkit PUBLIC HAVE_SDL2)
  target_link_libraries(taToolkit SDL2)
endif (HAVE_SDL2)
if (UNIX)
  target_link_libraries(taToolkit pthread)
endif (UNIX)
target_link_libraries(taToolkit freetype)
target_link_libraries(hello taToolkit)
