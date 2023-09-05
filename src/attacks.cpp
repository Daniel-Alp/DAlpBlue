#include "attacks.h"
#include "board.h"
#include "bitboard.h"
#include "types.h"
#include <array>
#include <cstdint>
#include <iostream>

bool sq_attacked(const Position& pos, const int sq, const Color side_attacking) {
	const uint64_t occluded = pos.all_bitboard | 0x8000000000000001;
	const uint64_t bishop_attacks = gen_bishop_attacks(sq, occluded);
	const uint64_t rook_attacks = gen_rook_attacks(sq, occluded);
	
	const Piece knight = build_pce(PieceType::KNIGHT, side_attacking);
	const Piece king = build_pce(PieceType::KING, side_attacking);
	const Piece bishop = build_pce(PieceType::BISHOP, side_attacking);
	const Piece rook = build_pce(PieceType::ROOK, side_attacking);
	const Piece queen = build_pce(PieceType::QUEEN, side_attacking);

	if (side_attacking == Color::WHITE) {
		if (black_pawn_attacks[sq] & pos.pce_bitboards[static_cast<int>(Piece::WHITE_PAWN)]) {
			return true;
		}
	}
	else {
		if (white_pawn_attacks[sq] & pos.pce_bitboards[static_cast<int>(Piece::BLACK_PAWN)]) {
			return true;
		}
	}
	if (knight_attacks[sq] & pos.pce_bitboards[static_cast<int>(knight)]) {
		return true;
	}
	if (king_attacks[sq] & pos.pce_bitboards[static_cast<int>(king)]) {
		return true;
	}
	if (bishop_attacks & (pos.pce_bitboards[static_cast<int>(bishop)] | pos.pce_bitboards[static_cast<int>(queen)])) {
		return true;
	}
	if (rook_attacks & (pos.pce_bitboards[static_cast<int>(rook)] | pos.pce_bitboards[static_cast<int>(queen)])) {
		return true;
	}
	return false;
}