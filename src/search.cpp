#include "attacks.h"
#include "board.h"
#include "evaluation.h"
#include "makemove.h"
#include "move.h"
#include "movegen.h"
#include "search.h"
#include <cstdint>
#include <iostream>

std::array<std::array<int64_t, 64>, 15> history_table;

void best_move(Position& pos, SearchData& search_data) {
	div_two_history_table();
	search_data.searching = true;
	search_data.nodes = 0;
	search_data.best_move_root = Move();
	
	for (int depth = 1; depth < search_data.max_depth; depth++) {
		pos.ply = 0;
		int32_t score = negamax(pos, search_data, -mate_score, mate_score, depth, 0, false);
		//std::cout << "info depth " << depth << " score cp " << score / material_midgame_vals[static_cast<int>(Piece::WHITE_PAWN)] << " pv " << best_move_root.to_str() << std::endl;
		if (!search_data.searching) {
			break;
		}
	}

	search_data.searching = false;
	std::cout << "bestmove " << search_data.best_move_root.to_str() << std::endl;
}

int32_t negamax(Position& pos, SearchData& search_data, int32_t alpha, int32_t beta, int depth, int ply, bool allow_null) {
	if (time_up(search_data)) {
		search_data.searching = false;
		return 0;
	}

	if (pos.ply >= search_data.max_depth) {
		return evaluate(pos);
	}

	const bool root_node = (ply == 0);
	const bool pv_node = (alpha != beta - 1);

	if (!root_node && (pos.fifty_move_rule >= 100 || repeated_pos(pos))) {
		return 0;
	}

	const HashEntry hash_entry = hash_table[pos.zobrist_key % num_hash_entries];
	const bool matching_hash_key = (hash_entry.zobrist_key == pos.zobrist_key);
	
	if (!pv_node && matching_hash_key && hash_entry.depth >= depth) {
		if (hash_entry.hash_flag == HashFlag::EXACT
			|| (hash_entry.hash_flag == HashFlag::BETA && hash_entry.score >= beta)
			|| (hash_entry.hash_flag == HashFlag::ALPHA && hash_entry.score <= alpha)) {
			return hash_entry.score;
		}
	}
	Move hash_entry_best_move = Move();
	if (matching_hash_key) {
		hash_entry_best_move = hash_entry.best_move;
	}

	if (depth <= 0) {
		return quiescence(pos, search_data, alpha, beta);
	}

	int32_t eval = evaluate(pos);

	const int king_sq = get_lsb(pos.pce_bitboards[static_cast<int>(build_pce(PieceType::KING, pos.side_to_move))]);
	const bool in_check = sq_attacked(pos, king_sq, flip_col(pos.side_to_move));

	if (!pv_node && !in_check) {
		if (eval - depth * 75 >= beta && depth < 9) {
			return eval;
		}

		if (allow_null && pos.phase_val > 2 && eval >= beta) {
			make_null_move(pos);
			const int reduction = 2;
			int32_t score = -negamax(pos, search_data, -beta, -beta + 1, depth - 1 - reduction, ply + 1, false);
			undo_null_move(pos);
			if (score >= beta) {
				return score;
			}
		}
	}

	MoveList move_list = gen_pseudo_moves(pos, false);
	int num_legal_moves = 0;

	std::array<int64_t, MoveList::max_moves> scores{};
	for (int i = 0; i < move_list.size(); i++) {
		scores[i] = score_move(move_list.get(i), hash_entry_best_move, pos.pces, ply);
	}

	int32_t best_score = -mate_score;
	Move best_move = Move();

	const int32_t orig_alpha = alpha;
	int32_t score;

	for (int i = 0; i < move_list.size(); i++) {
		const Move move = get_next_move(move_list, scores, i);
			
		if (!make_move(pos, move)) {
			continue;
		}

		num_legal_moves++;

		if (num_legal_moves > 1) {
			if (num_legal_moves >= 3 + 3 * pv_node
				&& depth >= 3
				&& !in_check
				&& move.is_quiet()) {

				int reduction = 2;

				if (depth - 1 - reduction <= 0) {
					reduction = depth - 2;
				}

				score = -negamax(pos, search_data, -alpha - 1, -alpha, depth - 1 - reduction, ply + 1, true);
			}
			else {
				score = alpha + 1;
			}
			if (score > alpha) {
				score = -negamax(pos, search_data, -alpha - 1, -alpha, depth - 1, ply + 1, true);
				if (score > alpha && score < beta) {
					score = -negamax(pos, search_data, -beta, -alpha, depth - 1, ply + 1, true);
				}
			}
		}
		else {
			score = -negamax(pos, search_data, -beta, -alpha, depth - 1, ply + 1, true);
		}

		undo_move(pos, move);

		if (!search_data.searching) {
			return 0;
		}

		if (score > best_score) {

			best_score = score;
			best_move = move;
			if (root_node) {
				search_data.best_move_root = move;
			}

			if (score > alpha) {
				alpha = score;
				if (score >= beta) {
					if (!move.is_quiet()) {
						break;
					}

					const Piece move_pce = pos.pces[move.get_from_sq()];
					history_table[static_cast<int>(move_pce)][move.get_to_sq()] += depth * depth;

					for (int j = 0; j < i; j++) {
						Move penalized_move = move_list.get(j);
						if (!penalized_move.is_quiet()) {
							continue;
						}
						const Piece penalized_move_pce = pos.pces[penalized_move.get_from_sq()];
						history_table[static_cast<int>(penalized_move_pce)][penalized_move.get_to_sq()] -= depth * depth;
					}

					break;
				}
			}
		}
	}

	if (num_legal_moves == 0) {
		if (in_check) {
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

int32_t quiescence(Position& pos, SearchData& search_data, int32_t alpha, int32_t beta) {
	if (time_up(search_data)) {
		search_data.searching = false;
		return 0;
	}

	int32_t best_score = evaluate(pos);

	if (best_score > alpha) {
		alpha = best_score;
		if (best_score >= beta) {
			return best_score;
		}
	}

	MoveList move_list = gen_pseudo_moves(pos, true);
	std::array<int64_t, MoveList::max_moves> scores{};
	for (int i = 0; i < move_list.size(); i++) {
		scores[i] = score_move(move_list.get(i), Move(), pos.pces, 0);
	}

	for (int i = 0; i < move_list.size(); i++) {
		const Move move = get_next_move(move_list, scores, i);

		if (!make_move(pos, move)) {
			continue;
		}

		int32_t score = -quiescence(pos, search_data, -beta, -alpha);
		undo_move(pos, move);
		if (time_up(search_data)) {
			return 0;
		}

		if (score > best_score) {
			best_score = score;

			if (score > alpha) {
				alpha = score;

				if (score >= beta) {
					break;
				}
			}
		}
	}

	return best_score;
}