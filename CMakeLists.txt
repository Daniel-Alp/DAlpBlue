# CMakeList.txt : CMake project for DAlpBlue, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto")

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()

project ("DAlpBlue")

# Add source to this project's executable.
add_executable (DAlpBlue "src/main.cpp" "src/board.cpp" "src/board.h" "src/types.h"  "src/types.cpp" )

# TODO: Add tests and install targets if needed.
