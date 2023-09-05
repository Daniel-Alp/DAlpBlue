#pragma once

#include "board.h"
#include "movegen.h"
#include "move.h"
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
int32_t negamax(Position& pos, SearchData& search_data, Move& best_move_root, int32_t alpha, int32_t beta, int depth, int ply);

constexpr inline int32_t mvv_lva(PieceType cap_pce_type, PieceType move_pce_type) {
	return static_cast<int>(cap_pce_type) * 64 - static_cast<int>(move_pce_type);
}

inline int32_t score_move(const Move& move, const Move& hash_entry_best_move, std::array<Piece, 64>& pces) {
	if (move == hash_entry_best_move) {
		return 15000;
	}
	else {
		Piece cap_pce = move.get_cap_pce();
		if (cap_pce != Piece::NONE) {
			return mvv_lva(get_pce_type(cap_pce), get_pce_type(pces[move.get_from_sq()]));
		}
	}
	return 0;
}

inline void get_next_move(MoveList& move_list, std::array<int32_t, MoveList::max_moves>& scores, int cur_move_index) {
	int best_move_index = cur_move_index;
	for (int i = cur_move_index + 1; i < move_list.size(); i++) {
		if (scores[i] > scores[best_move_index]) {
			best_move_index = i;
		}
	}
	move_list.swap(cur_move_index, best_move_index);
	std::swap(scores[cur_move_index], scores[best_move_index]);
}

inline bool time_up(SearchData& search_data) {
	search_data.nodes++;
	return (search_data.nodes & 2047) == 0 && get_current_time() - search_data.start_time > search_data.time_allotted;
}

inline bool repeated_pos(const Position& pos) {
	int min_ply = std::max(pos.history_ply - pos.fifty_move_rule, INT32_C(0));
	for (int ply = pos.history_ply - 2; ply >= min_ply; ply -= 2) {
		if (pos.zobrist_key == pos.history_stack[ply]) {
			return true;
		}
	}
	return false;
}