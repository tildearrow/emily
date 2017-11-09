# please add this subdirectory to your project.
# and link with target_link_libraries(target taToolkit)

cmake_minimum_required(VERSION 3.1)
project(emily)
set(CMAKE_CXX_STANDARD 11)

option(BUILD_STATIC "build static library instead" OFF)
option(BUILD_EXAMPLES "build sample programs" ON)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
if (BUILD_STATIC)
  set(BUILD_SHARED_LIBS FALSE)
else (BUILD_STATIC)
  set(BUILD_SHARED_LIBS TRUE)
endif (BUILD_STATIC)
add_subdirectory(sfml)

include_directories(include sfml/include)

find_library(USE_XCB xcb)

set(SOURCES src/funcs.cpp
            src/bitmap.cpp
            src/font.cpp
            
            src/widget/base.cpp
            src/widget/label.cpp)
if (APPLE)
  list(APPEND SOURCES src/nsstub.m)
endif (APPLE)
if (BUILD_STATIC)
  add_library(emily STATIC ${SOURCES})
else (BUILD_STATIC)
  add_library(emily SHARED ${SOURCES})
endif (BUILD_STATIC)
if (APPLE)
  set_target_properties(emily PROPERTIES FRAMEWORK TRUE MACOSX_FRAMEWORK_IDENTIFIER local.toolkit MACOSX_FRAMEWORK_INFO_PLIST Info.plist PUBLIC_HEADER toolkit.h)
endif (APPLE)
if (BUILD_EXAMPLES)
  add_executable(hello examples/hello/hello.cpp)
endif (BUILD_EXAMPLES)
if (USE_XCB)
  target_link_libraries(emily xcb)
  target_compile_definitions(emily PUBLIC USE_XCB)
endif (USE_XCB)
target_link_libraries(emily sfml-graphics sfml-window sfml-system)
if (UNIX)
  target_link_libraries(emily pthread)
endif (UNIX)
target_link_libraries(hello emily)
