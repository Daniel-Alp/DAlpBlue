# CMakeList.txt : CMake project for DAlpBlue, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.21)
project(DAlpBlue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()

project ("DAlpBlue")

# Add source to this project's executable.
add_executable (DAlpBlue "DAlpBlue.cpp" "DAlpBlue.h")

# TODO: Add tests and install targets if needed.
