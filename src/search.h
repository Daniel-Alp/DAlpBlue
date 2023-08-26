#pragma once

#include "board.h"
#include "movegen.h"
#include "transposition.h"
#include "timemanagement.h"

constexpr int32_t mate_score = 30000;

struct SearchData {
	bool searching;
	uint64_t start_time;
	uint64_t time_allotted;
	uint64_t nodes;
};

void get_best_move(Position& pos, SearchData& search_data);
void score_moves(std::array<uint32_t, max_moves>& moves, int num_moves, std::array<int32_t, max_moves>& scores, HashEntry& hash_entry, bool matching_hash_key);
void sort_moves(std::array<uint32_t, max_moves>& moves, int num_moves, std::array<int32_t, max_moves>& scores, int cur_move_index);
int32_t negamax(Position& pos, SearchData& search_data, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply);

inline bool time_up(SearchData& search_data) {
	search_data.nodes++;
	return (search_data.nodes & 2047) == 0 && get_current_time() - search_data.start_time > search_data.time_allotted;
}