#pragma once

#include "board.h"
#include "movegen.h"
#include "transposition.h"

constexpr int32_t mate_score = 30000;

struct SearchData {
	bool searching;
	uint64_t start_time;
	uint64_t time_allotted;
	uint64_t nodes;
};

void get_best_move(Position& pos, SearchData& search_data);
//bool is_repetition(Position& pos);
void score_moves(std::array<uint32_t, max_moves>& moves, int num_moves, HashEntry& hash_entry, bool matching_hash_key, std::array<int32_t, max_moves>& scores);
void order_moves(std::array<uint32_t, max_moves>& moves, int num_moves, std::array<int32_t, max_moves>& scores, int cur_move_index);
int32_t negamax(Position& pos, SearchData& search_data, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply, int32_t& best_score_root);