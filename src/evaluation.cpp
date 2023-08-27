#include "board.h"
#include "bitboard.h"
#include "evaluation.h"
#include "types.h"
#include <array>
#include <cstdint>

std::array<std::array<int32_t, 64>, 15> pce_psqts_midgame;
std::array<std::array<int32_t, 64>, 15> pce_psqts_endgame;

void precompute_pce_psqt(Piece piece, std::array<int32_t, 64> pcetype_psqt_midgame, std::array<int32_t, 64> pcetype_psqt_endgame) {
	Color col = get_col(piece);
	if (col == Color::WHITE) {
		for (int sq = 0; sq < 64; sq++) {
			pce_psqts_midgame[static_cast<int>(piece)][sq] = pcetype_psqt_midgame[sq];
			pce_psqts_endgame[static_cast<int>(piece)][sq] = pcetype_psqt_endgame[sq];
		}
	}
	else {
		for (int sq = 0; sq < 64; sq++) {
			pce_psqts_midgame[static_cast<int>(piece)][sq] = pcetype_psqt_midgame[sq ^ 56] * -1;
			pce_psqts_endgame[static_cast<int>(piece)][sq] = pcetype_psqt_endgame[sq ^ 56] * -1;
		}
	}
}

int32_t evaluate(Position& pos) {
	int32_t midgame_eval = pos.material_midgame_val + pos.psqt_midgame_val;
	int32_t endgame_eval = pos.material_endgame_val + pos.psqt_endgame_val;
	int phase = std::min(24, pos.phase_val);

	int32_t eval = (midgame_eval * phase + endgame_eval * (24 - phase)) / 24;

	if (pos.side_to_move == Color::BLACK) {
		eval *= -1;
	}

	return eval;
}