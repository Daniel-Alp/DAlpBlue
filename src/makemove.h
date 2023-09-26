#pragma once

#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "evaluation.h"
#include "move.h"
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
	const Piece pce = pos.pces[sq];
	const Color col = get_col(pce);
	const uint64_t sq_bb = get_sq_bitboard(sq);

	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, sq);

	pos.phase_val -= phase_vals[static_cast<int>(pce)];
	pos.material_midgame_val -= material_midgame_vals[static_cast<int>(pce)];
	pos.material_endgame_val -= material_endgame_vals[static_cast<int>(pce)];
	pos.psqt_midgame_val -= pce_psqts_midgame[static_cast<int>(pce)][sq];
	pos.psqt_endgame_val -= pce_psqts_endgame[static_cast<int>(pce)][sq];

	pos.pces[sq] = Piece::NONE;
	pos.pce_bitboards[static_cast<int>(pce)] = clr_sq(pos.pce_bitboards[static_cast<int>(pce)], sq_bb);
	pos.col_bitboards[static_cast<int>(col)] = clr_sq(pos.col_bitboards[static_cast<int>(col)], sq_bb);
	pos.all_bitboard = clr_sq(pos.all_bitboard, sq_bb);
}

constexpr void add_pce(Position& pos, Piece pce, int sq) {
	pos.pces[sq] = pce;
	const Color col = get_col(pce);
	const uint64_t sq_bb = get_sq_bitboard(sq);

	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, sq);

	pos.phase_val += phase_vals[static_cast<int>(pce)];
	pos.material_midgame_val += material_midgame_vals[static_cast<int>(pce)];
	pos.material_endgame_val += material_endgame_vals[static_cast<int>(pce)];
	pos.psqt_midgame_val += pce_psqts_midgame[static_cast<int>(pce)][sq];
	pos.psqt_endgame_val += pce_psqts_endgame[static_cast<int>(pce)][sq]; 

	pos.pces[sq] = pce;
	pos.pce_bitboards[static_cast<int>(pce)] = set_sq(pos.pce_bitboards[static_cast<int>(pce)], sq_bb);
	pos.col_bitboards[static_cast<int>(col)] = set_sq(pos.col_bitboards[static_cast<int>(col)], sq_bb);
	pos.all_bitboard = set_sq(pos.all_bitboard, sq_bb);
}

constexpr void move_pce(Position& pos, int from_sq, int to_sq) {
	const Piece pce = pos.pces[from_sq];
	const Color col = get_col(pce);
	const uint64_t from_sq_bb = get_sq_bitboard(from_sq);
	const uint64_t to_sq_bb = get_sq_bitboard(to_sq);

	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, from_sq);
	pos.zobrist_key = hash_pce(pos.zobrist_key, pce, to_sq);

	pos.psqt_midgame_val -= pce_psqts_midgame[static_cast<int>(pce)][from_sq];
	pos.psqt_endgame_val -= pce_psqts_endgame[static_cast<int>(pce)][from_sq];
	pos.psqt_midgame_val += pce_psqts_midgame[static_cast<int>(pce)][to_sq];
	pos.psqt_endgame_val += pce_psqts_endgame[static_cast<int>(pce)][to_sq];

	pos.pces[from_sq] = Piece::NONE;
	pos.pces[to_sq] = pce;
	pos.pce_bitboards[static_cast<int>(pce)] = move_sq(pos.pce_bitboards[static_cast<int>(pce)], from_sq_bb, to_sq_bb);
	pos.col_bitboards[static_cast<int>(col)] = move_sq(pos.col_bitboards[static_cast<int>(col)], from_sq_bb, to_sq_bb);
	pos.all_bitboard = move_sq(pos.all_bitboard, from_sq_bb, to_sq_bb);
}

constexpr void move_pce(Position& pos, Square from_sq, Square to_sq) {
	move_pce(pos, static_cast<int>(from_sq), static_cast<int>(to_sq));
}

inline void undo_null_move(Position& pos) {
	pos.history_ply--;
	pos.ply--;
	pos.side_to_move = flip_col(pos.side_to_move);

	pos.en_passant_sq	= pos.undo_stack[pos.ply].en_passant_sq;
	pos.castling_rights = pos.undo_stack[pos.ply].castling_rights;
	pos.fifty_move_rule = pos.undo_stack[pos.ply].fifty_move_rule;
	pos.zobrist_key		= pos.undo_stack[pos.ply].zobrist_key;
}

inline void make_null_move(Position& pos) {
	pos.undo_stack[pos.ply].en_passant_sq	= pos.en_passant_sq;
	pos.undo_stack[pos.ply].castling_rights = pos.castling_rights;
	pos.undo_stack[pos.ply].fifty_move_rule = pos.fifty_move_rule;
	pos.undo_stack[pos.ply].zobrist_key		= pos.zobrist_key;

	pos.history_stack[pos.history_ply] = pos.zobrist_key;

	if (pos.en_passant_sq != Square::NO_SQ) {
		pos.zobrist_key = hash_en_passant_sq(pos.zobrist_key, pos.en_passant_sq);
		pos.en_passant_sq = Square::NO_SQ;
	}
	pos.fifty_move_rule = 0;

	const Color move_col = pos.side_to_move;
	pos.side_to_move = flip_col(pos.side_to_move);
	pos.zobrist_key = hash_side_to_move(pos.zobrist_key);

	pos.history_ply++;
	pos.ply++;
}

bool make_move(Position& pos, const Move& move);
void undo_move(Position& pos, const Move& move);