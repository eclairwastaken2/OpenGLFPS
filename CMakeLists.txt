cmake_minimum_required(VERSION 3.20)
project(OpenGL3DWorld LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Dependencies.cmake)

add_compile_options(/showIncludes)

add_subdirectory(Core)
add_subdirectory(App)


