cmake_minimum_required(VERSION 3.20)
project(PolaroidFilter LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PolaroidFilter
    main.cpp
    glad.c
)
