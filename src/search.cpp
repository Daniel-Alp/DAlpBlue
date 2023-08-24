#include "attacks.h"
#include "evaluation.h"
#include "search.h"
#include "move.h"
#include "movegen.h"
#include "makemove.h"
#include <cstdint>
#include "cstdlib"
#include <iostream>

bool searching;

void get_best_move(Position& pos) {
	searching = true;
	pos.ply = 0;
	uint32_t best_move_root = null_move;
	negamax(pos, best_move_root, -mate_score, mate_score, 6, 0);
	searching = false;
	std::cout << "bestmove " << get_move_str(best_move_root) << std::endl;
}

int32_t negamax(Position& pos, uint32_t& best_move_root, int32_t alpha, int32_t beta, int depth, int ply) {
	if (depth <= 0) {
		return evaluate(pos);
	}

	std::array<uint32_t, max_moves> moves;
	int num_moves = 0;
	int num_legal_moves = 0;
	gen_pseudo_moves(pos, moves, num_moves, false);

	int32_t best_score = -mate_score;

	for (int i = 0; i < num_moves; i++) {
		int32_t move = moves[i];
		if (make_move(pos, move)) {
			num_legal_moves++;
			int32_t score = -negamax(pos, best_move_root, -beta, -alpha, depth - 1, ply + 1);
			undo_move(pos, move);

			if (score > best_score) {
				best_score = score;

				if (ply == 0) {
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
		int king_sq = get_lsb(pos.pce_bitboards[static_cast<uint32_t>(build_pce(PieceType::KING, pos.side_to_move))]);
		if (sq_attacked(pos, king_sq, flip_col(pos.side_to_move))) {
			return -mate_score + ply;
		}
		else {
			return 0;
		}
	}
	return best_score;
}