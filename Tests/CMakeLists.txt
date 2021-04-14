cmake_minimum_required(VERSION 3.10)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(".." inc)

file(GLOB SOURCES
    inc/*.h
    inc/*.hpp
    src/*.cpp
)

# set the project name
project(IambTests)

# add the executable
add_executable(IambTests ${SOURCES})
