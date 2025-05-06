cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINK_WHAT_YOU_USE ON)

project(simplify LANGUAGES C CXX)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "Please create a build directory and run CMake from there")
endif()

if(MSVC)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/EHsc>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>)
else()
  add_compile_options($<$<CXX_COMPILER_ID:GNU,Clang>:-fPIC>)
  add_compile_options($<$<CXX_COMPILER_ID:GNU,Clang>:-Wall$<SEMICOLON>-Wextra>)
  add_link_options($<$<CXX_COMPILER_ID:GNU,Clang>:-flto>)
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_USE_MATH_DEFINES)

add_executable(${PROJECT_NAME} test.cpp simplify.hpp)
