#pragma once

#include "bitboard.h"
#include "board.h"
#include "evaluation.h"
#include "zobrist.h"
#include <array>
#include <cstdint>

constexpr std::array<int, 64> castling = {
	13, 15, 15, 15, 12, 15, 15, 14,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	7,  15, 15, 15,  3, 15, 15, 11
};

constexpr void clr_pce(Position& pos, int sq) {
	Piece pce = pos.pces[sq];
	Color col = get_col(pce);
	uint64_t sq_bb = get_sq_bitboard(sq);

	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, sq);

	//pos.phase_val -= phase_vals[static_cast<int>(pce)];
	//pos.material_midgame_val -= material_midgame_vals[static_cast<int>(pce)];
	//pos.material_endgame_val -= material_endgame_vals[static_cast<int>(pce)];
	//pos.psqt_midgame_val -= pce_psqts_midgame[static_cast<int>(pce)][sq];
	//pos.psqt_endgame_val -= pce_psqts_endgame[static_cast<int>(pce)][sq];

	pos.pces[sq] = Piece::NONE;
	pos.pce_bitboards[static_cast<int>(pce)] = clr_sq(pos.pce_bitboards[static_cast<int>(pce)], sq_bb);
	pos.col_bitboards[static_cast<int>(col)] = clr_sq(pos.col_bitboards[static_cast<int>(col)], sq_bb);
	pos.all_bitboard = clr_sq(pos.all_bitboard, sq_bb);
}

constexpr void add_pce(Position& pos, Piece& pce, int sq) {
	pos.pces[sq] = pce;
	Color col = get_col(pce);
	uint64_t sq_bb = get_sq_bitboard(sq);

	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, sq);

	//pos.phase_val += phase_vals[static_cast<int>(pce)];
	//pos.material_midgame_val += material_midgame_vals[static_cast<int>(pce)];
	//pos.material_endgame_val += material_endgame_vals[static_cast<int>(pce)];
	//pos.psqt_midgame_val += pce_psqts_midgame[static_cast<int>(pce)][sq];
	//pos.psqt_endgame_val += pce_psqts_endgame[static_cast<int>(pce)][sq];

	pos.pces[sq] = pce;
	pos.pce_bitboards[static_cast<int>(pce)] = set_sq(pos.pce_bitboards[static_cast<int>(pce)], sq_bb);
	pos.col_bitboards[static_cast<int>(col)] = set_sq(pos.col_bitboards[static_cast<int>(col)], sq_bb);
	pos.all_bitboard = set_sq(pos.all_bitboard, sq_bb);
}

constexpr void move_pce(Position& pos, int from_sq, int to_sq) {
	Piece pce = pos.pces[from_sq];
	Color col = get_col(pce);
	uint64_t from_sq_bb = get_sq_bitboard(from_sq);
	uint64_t to_sq_bb = get_sq_bitboard(to_sq);

	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, from_sq);
	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, to_sq);

	//pos.psqt_midgame_val -= pce_psqts_midgame[static_cast<int>(pce)][from_sq];
	//pos.psqt_endgame_val -= pce_psqts_endgame[static_cast<int>(pce)][from_sq];
	//pos.psqt_midgame_val += pce_psqts_midgame[static_cast<int>(pce)][to_sq];
	//pos.psqt_endgame_val += pce_psqts_endgame[static_cast<int>(pce)][to_sq];

	pos.pces[from_sq] = Piece::NONE;
	pos.pces[to_sq] = pce;
	pos.pce_bitboards[static_cast<int>(pce)] = move_sq(pos.pce_bitboards[static_cast<int>(pce)], from_sq_bb, to_sq_bb);
	pos.col_bitboards[static_cast<int>(col)] = move_sq(pos.col_bitboards[static_cast<int>(col)], from_sq_bb, to_sq_bb);
	pos.all_bitboard = move_sq(pos.all_bitboard, from_sq_bb, to_sq_bb);
}
bool make_move(Position& pos, uint32_t move);
void undo_move(Position& pos, uint32_t move);