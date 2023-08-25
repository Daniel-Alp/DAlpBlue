#include "board.h"
#include "bitboard.h"
#include "evaluation.h"
#include "types.h"
#include <array>
#include <cstdint>

#include <iostream>

int32_t evaluate(Position& pos) {
	int32_t midgame_eval = 0;
	for (int col = static_cast<int>(Color::WHITE); col <= static_cast<int>(Color::BLACK); col++) {
		for (int pce_type = static_cast<int>(PieceType::PAWN); pce_type <= static_cast<int>(PieceType::KING); pce_type++) {
			uint64_t pce_bitboard = pos.pce_bitboards[build_pce(pce_type, col)];
			while (pce_bitboard != 0) {
				midgame_eval += midgame_pce_vals[pce_type];
				pce_bitboard = clr_lsb(pce_bitboard);
			}
		}
		midgame_eval *= -1;
	}
	if (pos.side_to_move == Color::BLACK) {
		midgame_eval *= -1;
	}
	return midgame_eval;
}