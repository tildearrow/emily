# please add this subdirectory to your project.
# and link with target_link_libraries(target emily)

cmake_minimum_required(VERSION 3.1)
project(emily)
set(CMAKE_CXX_STANDARD 11)

option(BUILD_STATIC "build static library instead" OFF)
option(BUILD_EXAMPLES "build sample programs" ON)

if (WIN32)
  set(BUILD_STATIC ON)
endif (WIN32)

# legacy. it has always worked well.
# heck, it is not even legacy. you're just ignoring AMD here.
set(OpenGL_GL_PREFERENCE "LEGACY")
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
if (BUILD_STATIC)
  set(BUILD_SHARED_LIBS FALSE)
else (BUILD_STATIC)
  set(BUILD_SHARED_LIBS TRUE)
endif (BUILD_STATIC)
add_subdirectory(sfml)

if (APPLE OR WIN32)
  include_directories(include sfml/include sfml/extlibs/headers/freetype2)
else (APPLE OR WIN32)
  include_directories(include sfml/include /usr/include/freetype2)
endif (APPLE OR WIN32)

if (NOT APPLE)
find_library(USE_XCB xcb)
find_library(HAVE_FONTCONFIG fontconfig)
endif (NOT APPLE)

if (WIN32)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  find_library(HAVE_FT2 freetype PATHS sfml/extlibs/libs-mingw/x86)
else (WIN32)
  find_library(HAVE_FT2 freetype PATHS sfml/extlibs)
endif (WIN32)

if (NOT HAVE_FT2)
  message("you don't have freetype2. this is a problem.")
endif (NOT HAVE_FT2)

set(SOURCES src/funcs.cpp
            src/bitmap.cpp
            src/font.cpp
            src/frameDispatch.cpp
	    src/log.cpp
            src/string.cpp
            src/icon.cpp
            src/cmenu.cpp
            src/video.cpp
            
            src/skin/default.cpp
            
            src/widget/base.cpp
            src/widget/label.cpp
            src/widget/button.cpp
            src/widget/light.cpp
            src/widget/frameview.cpp
            src/widget/slider.cpp)
if (APPLE)
  list(APPEND SOURCES src/nsstub.m)
  find_library(HAVE_APPKIT AppKit PATHS ${CMAKE_OSX_SYSROOT}/System/Library PATH_SUFFIXES Frameworks NO_DEFAULT_PATH)
endif (APPLE)
if (BUILD_STATIC)
  add_library(emily STATIC ${SOURCES})
else (BUILD_STATIC)
  add_library(emily SHARED ${SOURCES})
endif (BUILD_STATIC)
if (APPLE)
  set_target_properties(emily PROPERTIES FRAMEWORK TRUE MACOSX_FRAMEWORK_IDENTIFIER local.emily MACOSX_FRAMEWORK_INFO_PLIST ${PROJECT_SOURCE_DIR}/Info.plist PUBLIC_HEADER toolkit.h)
endif (APPLE)
if (BUILD_EXAMPLES)
  add_executable(hello examples/hello/hello.cpp)
  add_executable(framenest examples/framenest/framenest.cpp)
endif (BUILD_EXAMPLES)
if (USE_XCB)
  target_link_libraries(emily xcb)
  target_compile_definitions(emily PUBLIC USE_XCB)
endif (USE_XCB)
if (USE_XCB)
  target_link_libraries(emily fontconfig)
  target_compile_definitions(emily PUBLIC HAVE_FONTCONFIG)
endif (USE_XCB)

#add_custom_command(TARGET emily WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/fa COMMAND xxd -i FontAwesome.otf > fa.h)
target_link_libraries(emily sfml-graphics sfml-window sfml-system ${HAVE_FT2})
if (UNIX)
  target_link_libraries(emily pthread)
endif (UNIX)
if (APPLE)
  target_link_libraries(emily ${HAVE_APPKIT})
endif (APPLE)
if (BUILD_EXAMPLES)
  target_link_libraries(hello emily)
  target_link_libraries(framenest emily)
endif (BUILD_EXAMPLES)
