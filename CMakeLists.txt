# CMakeList.txt : CMake project for DAlpBlue, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.21)
project (DAlpBlue)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE RelWithDebugInfo)
endif()

# Add source to this project's executable.
add_executable (DAlpBlue "src/main.cpp" "src/board.cpp" "src/board.h" "src/types.h"  "src/types.cpp"  "src/bitboard.h" "src/bitboard.cpp" "src/movegen.cpp" "src/movegen.h" "src/move.cpp" "src/move.h" "src/attacks.cpp" "src/attacks.h" "src/makemove.h" "src/makemove.cpp" "src/perft.h" "src/perft.cpp")

# TODO: Add tests and install targets if needed.
