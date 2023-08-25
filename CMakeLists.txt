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
add_executable (DAlpBlue "src/main.cpp" "src/board.cpp" "src/board.h" "src/types.h"  "src/types.cpp"  "src/bitboard.h" "src/bitboard.cpp" "src/movegen.cpp" "src/movegen.h" "src/move.cpp" "src/move.h" "src/attacks.cpp" "src/attacks.h" "src/makemove.h" "src/makemove.cpp" "src/perft.h" "src/perft.cpp" "src/parser.cpp" "src/parser.h"   "src/uci.cpp" "src/uci.h" "src/evaluation.cpp" "src/evaluation.h" "src/search.cpp" "src/search.h"  "src/zobrist.cpp" "src/zobrist.h" "src/transposition.cpp" "src/transposition.h" "src/timemanagement.cpp" "src/timemanagement.h")

# TODO: Add tests and install targets if needed.
