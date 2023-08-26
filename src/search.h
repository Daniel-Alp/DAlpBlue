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
int32_t negamax(Position& pos, SearchData& search_data, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply);

constexpr int32_t score_move(uint32_t move, uint32_t hash_entry_best_move) {
	if (move == hash_entry_best_move) {
		return 15000;
	}
	else {
		return 0;
	}
}

inline void sort_moves(std::array<uint32_t, max_moves>& moves, int num_moves, std::array<int32_t, max_moves>& scores, int cur_move_index) {
	int best_move_index = cur_move_index;
	for (int i = cur_move_index + 1; i < num_moves; i++) {
		if (scores[i] > scores[best_move_index]) {
			best_move_index = i;
		}
	}
	std::swap(moves[cur_move_index], moves[best_move_index]);
	std::swap(scores[cur_move_index], scores[best_move_index]);
}
inline bool time_up(SearchData& search_data) {
	search_data.nodes++;
	return (search_data.nodes & 2047) == 0 && get_current_time() - search_data.start_time > search_data.time_allotted;
}