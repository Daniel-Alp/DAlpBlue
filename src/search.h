#pragma once

#include "board.h"
#include "constants.h"
#include "evaluation.h"
#include "movegen.h"
#include "move.h"
#include "transposition.h"
#include "timemanagement.h"
#include <algorithm>

extern std::array<std::array<int64_t, 64>, 15> history_table;
extern std::array<std::array<Move, 2>, 257> killer_table;
extern std::array<std::array<int, 218>, 256> reduction_table;

struct SearchData {
	bool searching;
	int max_depth;
	uint64_t start_time;
	uint64_t time_allotted;
	uint64_t nodes;
	Move best_move_root;
};

void best_move(Position& pos, SearchData& search_data);
int32_t negamax(Position& pos, SearchData& search_data, int32_t alpha, int32_t beta, int depth, int ply, bool allow_null);
int32_t quiescence(Position& pos, SearchData& search_data, int32_t alpha, int32_t beta);

inline void precompute_reduction_table() {
	for (int depth = 1; depth < 256; depth++) {
		for (int num_moves = 1; num_moves < 218; num_moves++) {
			reduction_table[depth][num_moves] = std::max((log(depth) * log(num_moves)) / 2 + 1, static_cast<double>(2));
		}
	}
}

inline void clr_history_table() {
	for (int move_pce = Piece::NONE; move_pce <= Piece::BLACK_KING; move_pce++) {
		for (int to_sq = 0; to_sq < 64; to_sq++) {
			history_table[move_pce][to_sq] = 0;
		}
	}
}

inline void div_two_history_table() {
	for (int move_pce = 0; move_pce <= Piece::BLACK_KING; move_pce++) {
		for (int to_sq = 0; to_sq < 64; to_sq++) {
			history_table[move_pce][to_sq] /= 2;
		}
	}
}

inline void clr_killer_table() {
	for (int ply = 0; ply < 256; ply++) {
		killer_table[ply][0] = Move();
		killer_table[ply][1] = Move();
	}
}

constexpr int64_t mvv_lva(PieceType cap_pce_type, PieceType move_pce_type) {
	return (static_cast<int64_t>(cap_pce_type) << 50) - static_cast<int64_t>(move_pce_type);
}

inline int64_t score_move(const Move& move, const Move& hash_entry_best_move, std::array<Piece, 64>& pces, int ply) {
	if (move == hash_entry_best_move) {
		return INT64_C(1) << 60;
	}
	if (move == killer_table[ply][1]) {
		return INT64_C(1) << 41;
	}
	if (move == killer_table[ply][0]) {
		return INT64_C(1) << 40;
	}

	const Piece move_pce = pces[move.get_from_sq()];
	const Piece cap_pce = move.get_cap_pce();
	const PieceType move_pce_type = get_pce_type(pces[move_pce]);
	
	if (cap_pce != Piece::NONE) {
		return mvv_lva(get_pce_type(cap_pce), move_pce_type);
	}

	const int from_sq = move.get_from_sq();
	const int to_sq = move.get_to_sq();

	return history_table[move_pce][to_sq] + pce_psqts_midgame[move_pce_type][to_sq] - pce_psqts_midgame[move_pce_type][from_sq];
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

inline std::string get_info_str(SearchData& search_data, int depth, int32_t score) {
	std::string info_str = "info depth " + std::to_string(depth);

	if (score >= mate_score - search_data.max_depth) {
		info_str += " score mate " + std::to_string((mate_score - score + 1) / 2);
	}
	else if (score <= -mate_score + search_data.max_depth) {
		info_str += " score mate " + std::to_string((-mate_score - score - 1) / 2);
	}
	else {
		info_str += " score cp " + std::to_string(100 * score / material_midgame_vals[Piece::WHITE_PAWN]);
	}
	info_str += " pv " + search_data.best_move_root.to_str();

	return info_str;
}