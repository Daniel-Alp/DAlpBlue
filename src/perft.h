#pragma once

#include "board.h"
#include <cstdint>

uint64_t run_perft(Position& pos, int depth, int ply);
uint64_t run_perft_with_hash_table(Position& pos, int depth, int ply);
void run_perft_suite();