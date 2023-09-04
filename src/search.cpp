#include "attacks.h"
#include "board.h"
#include "evaluation.h"
#include "makemove.h"
#include "move.h"
#include "movegen.h"
#include "search.h"
#include <cstdint>
#include <iostream>

void get_best_move(Position& pos, SearchData& search_data) {
	Move best_move_root = Move();
	search_data.searching = true;
	search_data.nodes = 0;

	for (int depth = 1; depth < 255; depth++) {
		pos.ply = 0;
		const int32_t score = negamax(pos, search_data, best_move_root, -mate_score, mate_score, depth, 0);
		if (!search_data.searching) {
			break;
		}
	}

	search_data.searching = false;
	std::cout << "bestmove " << best_move_root.to_str() << std::endl;
}

int32_t negamax(Position& pos, SearchData& search_data, Move& best_move_root, int32_t alpha, int32_t beta, int depth, int ply) {
	if (time_up(search_data)) {
		search_data.searching = false;
		return 0;

	}

	bool root_node = (ply == 0);
	bool pv_node = (alpha != beta - 1);

	if (!root_node && repeated_pos(pos)) {
		return 0;
	}

	HashEntry hash_entry = hash_table[pos.zobrist_key % num_hash_entries];
	bool matching_hash_key = (hash_entry.zobrist_key == pos.zobrist_key);

	if (!root_node && !pv_node && matching_hash_key && hash_entry.depth >= depth) {
		if (hash_entry.hash_flag == HashFlag::EXACT
			|| (hash_entry.hash_flag == HashFlag::BETA && hash_entry.score >= beta)
			|| (hash_entry.hash_flag == HashFlag::ALPHA && hash_entry.score <= alpha)) {
			return hash_entry.score;
		}	
	}

	if (depth <= 0) {
		return evaluate(pos);
	}

	std::array<Move, max_moves> moves;
	int num_moves = 0;
	int num_legal_moves = 0;
	gen_pseudo_moves(pos, moves, num_moves, false);

	std::array<int32_t, max_moves> scores{};
	Move hash_entry_best_move = Move();
	if (matching_hash_key) {
		hash_entry_best_move = hash_entry.best_move;
	}
	for (int i = 0; i < num_moves; i++) {
		scores[i] = score_move(moves[i], hash_entry_best_move, pos.pces);
	}

	int32_t best_score = -mate_score;
	Move best_move = Move();

	const int32_t orig_alpha = alpha;
	int32_t score;

	for (int i = 0; i < num_moves; i++) {
		get_next_move(moves, num_moves, scores, i);
		const Move move = moves[i];

		if (make_move(pos, move)) {
			num_legal_moves++;

			if (num_legal_moves > 1) {
				score = -negamax(pos, search_data, best_move_root, -alpha - 1, -alpha, depth - 1, ply + 1);
				if (score > alpha && score < beta) {
					score = -negamax(pos, search_data, best_move_root, -beta, -alpha, depth - 1, ply + 1);
				}
			}
			else {
				score = -negamax(pos, search_data, best_move_root, -beta, -alpha, depth - 1, ply + 1);
			}

			undo_move(pos, move);

			if (!search_data.searching) {
				return 0;
			}

			if (score > best_score) {
				best_score = score;
				best_move = move;

				if (root_node) {
					best_move_root = move;
				}

				if (score > alpha) {
					alpha = score;

					if (score >= beta) {
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
	if (best_score >= beta) {
		hash_flag = HashFlag::BETA;
	}
	else if (best_score > orig_alpha) {
		hash_flag = HashFlag::EXACT;
	}
	else {
		hash_flag = HashFlag::ALPHA;
	}
	record_hash_entry(pos.zobrist_key, depth, best_score, hash_flag, best_move);

	return best_score;
}