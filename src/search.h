#pragma once

#include "board.h"
#include "evaluation.h"
#include "movegen.h"
#include "move.h"
#include "transposition.h"
#include "timemanagement.h"

constexpr int32_t mate_score = 30000;
extern std::array<std::array<int64_t, 64>, 15> history_table;
extern std::array<std::array<Move, 2>, 256> killer_table; //Currently unused



struct SearchData {
	bool searching;
	int max_depth;
	uint64_t start_time;
	uint64_t time_allotted;
	uint64_t nodes;
};

void best_move(Position& pos, SearchData& search_data);
int32_t negamax(Position& pos, SearchData& search_data, Move& best_move_root, int32_t alpha, int32_t beta, int depth, int ply, bool allow_null);
int32_t quiescence(Position& pos, SearchData& search_data, int32_t alpha, int32_t beta);

inline void clr_history_table() {
	for (int move_pce = 0; move_pce <= static_cast<int>(Piece::BLACK_KING); move_pce++) {
		for (int to_sq = 0; to_sq < 64; to_sq++) {
			history_table[move_pce][to_sq] = 0;
		}
	}
}

inline void div_two_history_table() {
	for (int move_pce = 0; move_pce <= static_cast<int>(Piece::BLACK_KING); move_pce++) {
		for (int to_sq = 0; to_sq < 64; to_sq++) {
			history_table[move_pce][to_sq] /= 2;
		}
	}
}

constexpr int64_t mvv_lva(PieceType cap_pce_type, PieceType move_pce_type) {
	return static_cast<int64_t>(cap_pce_type) << 50 - static_cast<int64_t>(move_pce_type);
}

inline int64_t score_move(const Move& move, const Move& hash_entry_best_move, std::array<Piece, 64>& pces, int ply) {
	if (move == hash_entry_best_move) {
		return INT64_C(1) << 60;
	}

	const Piece move_pce = pces[move.get_from_sq()];
	const PieceType move_pce_type = get_pce_type(pces[static_cast<int>(move_pce)]);
	const Piece cap_pce = move.get_cap_pce();
	if (cap_pce != Piece::NONE) {
		return mvv_lva(get_pce_type(cap_pce), move_pce_type);
	}

	const int from_sq = move.get_to_sq();
	const int to_sq = move.get_to_sq();
	return history_table[static_cast<int>(move_pce)][to_sq] + pce_psqts_midgame[static_cast<int>(move_pce_type)][to_sq] - pce_psqts_midgame[static_cast<int>(move_pce_type)][from_sq];
}

inline Move get_next_move(MoveList& move_list, std::array<int64_t, MoveList::max_moves>& scores, int cur_move_index) {
	int best_move_index = cur_move_index;
	for (int i = cur_move_index + 1; i < move_list.size(); i++) {
		if (scores[i] > scores[best_move_index]) {
			best_move_index = i;
		}
	}
	move_list.swap(cur_move_index, best_move_index);
	std::swap(scores[cur_move_index], scores[best_move_index]);
	return move_list.get(cur_move_index);
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