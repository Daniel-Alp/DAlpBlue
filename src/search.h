#pragma once

#include "board.h"

extern bool searching;

constexpr int32_t mate_score = 30000;

void get_best_move(Position& pos);
int32_t negamax(Position& pos, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply);