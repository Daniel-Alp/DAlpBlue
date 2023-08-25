#include "attacks.h"
#include "board.h"
#include "bitboard.h"
#include "types.h"
#include <array>
#include <cstdint>
#include <iostream>

bool sq_attacked(Position& pos, int sq, Color side_attacking) {
	uint64_t occluded = pos.all_bitboard | 0x8000000000000001;
	uint64_t bishop_attacks = gen_bishop_attacks(sq, occluded);
	uint64_t rook_attacks = gen_rook_attacks(sq, occluded);
	
	Piece knight = build_pce(PieceType::KNIGHT, side_attacking);
	Piece king = build_pce(PieceType::KING, side_attacking);
	Piece bishop = build_pce(PieceType::BISHOP, side_attacking);
	Piece rook = build_pce(PieceType::ROOK, side_attacking);
	Piece queen = build_pce(PieceType::QUEEN, side_attacking);

	if (side_attacking == Color::WHITE) {
		if (black_pawn_attacks[sq] & pos.pce_bitboards[static_cast<uint32_t>(Piece::WHITE_PAWN)]) {
			return true;
		}
	}
	else {
		if (white_pawn_attacks[sq] & pos.pce_bitboards[static_cast<uint32_t>(Piece::BLACK_PAWN)]) {
			return true;
		}
	}
	if (knight_attacks[sq] & pos.pce_bitboards[static_cast<uint32_t>(knight)]) {
		return true;
	}
	if (king_attacks[sq] & pos.pce_bitboards[static_cast<uint32_t>(king)]) {
		return true;
	}
	if (bishop_attacks & (pos.pce_bitboards[static_cast<uint32_t>(bishop)] | pos.pce_bitboards[static_cast<uint32_t>(queen)])) {
		return true;
	}
	if (rook_attacks & (pos.pce_bitboards[static_cast<uint32_t>(rook)] | pos.pce_bitboards[static_cast<uint32_t>(queen)])) {
		return true;
	}
	return false;
}

void print_attacks(Position& pos, Color side_attacking) {
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			if (file == 0) {
				std::cout << rank + 1 << "   ";
			}
			if (sq_attacked(pos, get_sq(rank, file), side_attacking)) {
				std::cout << "X ";
			}
			else {
				std::cout << ". ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "    a b c d e f g h" << std::endl << std::endl;
}