#pragma once

#include "board.h"
#include "movegen.h"
#include "transposition.h"

extern bool searching;

constexpr int32_t mate_score = 30000;

void get_best_move(Position& pos);
void score_moves(std::array<uint32_t, max_moves>& moves, int num_moves, HashEntry& hash_entry, bool matching_hash_key, std::array<uint32_t, max_moves>& scores);
void order_moves(std::array<uint32_t, max_moves>& moves, int num_moves, std::array<uint32_t, max_moves>& scores, int cur_move_index);
int32_t negamax(Position& pos, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply);