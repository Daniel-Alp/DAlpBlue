#pragma once

#include "board.h"
#include "bitboard.h"
#include "types.h"
#include <array>
#include <cstdint>

#include <iostream>

inline uint64_t gen_bishop_attacks(const int sq, uint64_t occluded) {
	occluded |= 0x8000000000000001;
	const int block_noea = get_lsb(ray_noea_stop[sq] & occluded);
	const int block_nowe = get_lsb(ray_nowe_stop[sq] & occluded);
	const int block_soea = get_msb(ray_soea_stop[sq] & occluded);
	const int block_sowe = get_msb(ray_sowe_stop[sq] & occluded);
	return ray_bishop[sq] ^ ray_noea[block_noea] ^ ray_nowe[block_nowe] ^ ray_soea[block_soea] ^ ray_sowe[block_sowe];
}

inline uint64_t gen_rook_attacks(const int sq, uint64_t occluded) {
	occluded |= 0x8000000000000001;
	const int block_nort = get_lsb(ray_nort_stop[sq] & occluded);
	const int block_east = get_lsb(ray_east_stop[sq] & occluded);
	const int block_sout = get_msb(ray_sout_stop[sq] & occluded);
	const int block_west = get_msb(ray_west_stop[sq] & occluded);
	return ray_rook[sq] ^ ray_nort[block_nort] ^ ray_east[block_east] ^ ray_sout[block_sout] ^ ray_west[block_west];
}

inline bool sq_attacked(const Position& pos, const int sq, const Color side_attacking) {
	const uint64_t occluded = pos.all_bitboard | 0x8000000000000001;
	const uint64_t bishop_attacks = gen_bishop_attacks(sq, occluded);
	const uint64_t rook_attacks = gen_rook_attacks(sq, occluded);

	const Piece knight = build_pce(PieceType::KNIGHT, side_attacking);
	const Piece king   = build_pce(PieceType::KING, side_attacking);
	const Piece bishop = build_pce(PieceType::BISHOP, side_attacking);
	const Piece rook   = build_pce(PieceType::ROOK, side_attacking);
	const Piece queen  = build_pce(PieceType::QUEEN, side_attacking);

	if (side_attacking == Color::WHITE) {
		if (black_pawn_attacks[sq] & pos.pce_bitboards[Piece::WHITE_PAWN]) {
			return true;
		}
	}
	else {
		if (white_pawn_attacks[sq] & pos.pce_bitboards[Piece::BLACK_PAWN]) {
			return true;
		}
	}
	if (knight_attacks[sq] & pos.pce_bitboards[knight]) {
		return true;
	}
	if (king_attacks[sq] & pos.pce_bitboards[king]) {
		return true;
	}
	if (bishop_attacks & (pos.pce_bitboards[bishop] | pos.pce_bitboards[queen])) {
		return true;
	}

	if (rook_attacks & (pos.pce_bitboards[rook] | pos.pce_bitboards[queen])) {
		return true;
	}
	return false;
}