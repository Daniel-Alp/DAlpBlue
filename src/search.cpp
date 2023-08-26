#include "attacks.h"
#include "evaluation.h"
#include "search.h"
#include "move.h"
#include "movegen.h"
#include "makemove.h"
#include "timemanagement.h"
#include "transposition.h"
#include <cstdint>
#include "cstdlib"
#include <iostream>

void get_best_move(Position& pos, SearchData& search_data) {
	uint32_t best_move_root = null_move;

	for (int depth = 1; depth < 256; depth++) {
		pos.ply = 0;
		negamax(pos, search_data, best_move_root, -mate_score, mate_score, depth, 0);
		if (!search_data.searching) {
			break;
		}
	}
	search_data.searching = false;
	std::cout << "bestmove " << get_move_str(best_move_root) << std::endl;
}

void score_moves(std::array<uint32_t, max_moves>& moves, int num_moves, HashEntry& hash_entry, bool matching_hash_key, std::array<int32_t, max_moves>& scores) {
	for (int i = 0; i < num_moves; i++) {
		scores[i] = 0;
		if (matching_hash_key && moves[i] == hash_entry.best_move) {
			scores[i] = 60000;
		}
	}
}

void order_moves(std::array<uint32_t, max_moves>& moves, int num_moves, std::array<int32_t, max_moves>& scores, int cur_move_index) {
	int best_move_index = cur_move_index;
	for (int i = cur_move_index + 1; i < num_moves; i++) {
		if (scores[i] > scores[best_move_index]) {
			best_move_index = i;
		}
	}
	std::swap(scores[cur_move_index], scores[best_move_index]);
	std::swap(moves[cur_move_index], moves[best_move_index]);
}

int32_t negamax(Position& pos, SearchData& search_data, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply) {
	search_data.nodes++;
	if ((search_data.nodes % 2048) == 0 && get_current_time() - search_data.start_time > search_data.time_allotted) {
		search_data.searching = false;
		return 0;
	}

	bool root_node = (ply == 0);
	
	HashEntry hash_entry = hash_table[pos.zobrist_key % num_hash_entries];
	bool matching_hash_key = (hash_entry.zobrist_key == pos.zobrist_key);

	if (!root_node && matching_hash_key && hash_entry.depth >= depth) {
		if (hash_entry.hash_flag == HashFlag::EXACT
			|| (hash_entry.hash_flag == HashFlag::BETA && hash_entry.score >= beta)
			|| (hash_entry.hash_flag == HashFlag::ALPHA && hash_entry.score <= alpha)) {
			return hash_entry.score;
		}
	}

	if (depth <= 0) {
		return quiescence(pos, search_data, alpha, beta);
	}

	std::array<uint32_t, max_moves> moves;
	int num_moves = 0;
	int num_legal_moves = 0;
	gen_pseudo_moves(pos, moves, num_moves, false);

	std::array<int32_t, max_moves> scores{};
	score_moves(moves, num_moves, hash_entry, matching_hash_key, scores);

	int32_t best_score = -mate_score;
	uint32_t best_move = null_move;

	int32_t orig_alpha = alpha;

	for (int i = 0; i < num_moves; i++) {
		order_moves(moves, num_moves, scores, i);
		uint32_t move = moves[i];

		if (make_move(pos, move)) {
			num_legal_moves++;
			int32_t curr_score = -negamax(pos, search_data, best_move_root, -beta, -alpha, depth - 1, ply + 1);
			undo_move(pos, move);
			if (!search_data.searching) {
				return 0;	
			}

			if (curr_score > best_score) {
				best_score = curr_score;
				best_move = move;

				if (root_node) {
					best_move_root = move;
				}

				if (curr_score > alpha) {
					alpha = curr_score;

					if (curr_score >= beta) {
						break;
					}
				}
			}
		}
	}
	if (num_legal_moves == 0) {
		int king_sq = get_lsb(pos.pce_bitboards[static_cast<int>(build_pce(PieceType::KING, pos.side_to_move))]);
		if (sq_attacked(pos, king_sq, flip_col(pos.side_to_move))) {
			return -mate_score + ply;
		}
		else {
			return 0;
		}
	}

	HashFlag hash_flag;
	if (best_score <= orig_alpha) {
		hash_flag = HashFlag::ALPHA;
	}
	else if (best_score >= beta) {
		hash_flag = HashFlag::BETA;
	}
	else {
		hash_flag = HashFlag::EXACT;
	}
	record_hash_entry(pos.zobrist_key, depth, best_score, hash_flag, best_move);

	return best_score;
}

int32_t quiescence(Position& pos, SearchData& search_data, int32_t alpha, int32_t beta) {
	if ((search_data.nodes % 2048) == 0 && get_current_time() - search_data.start_time > search_data.time_allotted) {
		search_data.searching = false;
		return 0;
	}
	
	int32_t best_score = evaluate(pos);
	if (best_score > alpha) {
		alpha = best_score;
	}
	if (alpha >= beta) {
		return best_score;
	}

	std::array<uint32_t, max_moves> moves;
	int num_moves = 0;
	gen_pseudo_moves(pos, moves, num_moves, true);

	for (int i = 0; i < num_moves; i++) {
		uint32_t move = moves[i];

		if (make_move(pos, move)) {
			int32_t curr_score = -quiescence(pos, search_data, -beta, -alpha);
			undo_move(pos, move);
			if (!search_data.searching) {
				return 0;
			}

			if (curr_score > best_score) {
				best_score = curr_score;

				if (curr_score > alpha) {
					alpha = curr_score;

					if (curr_score >= beta) {
						break;
					}
				}
			}
		}
	}
	return best_score;
}