#include "board.h"
#include "types.h"
#include "zobrist.h"

uint64_t key_side_to_move;
std::array<std::array<uint64_t, 64>, 15> keys_pce{};
std::array<uint64_t, 16> keys_castling_rights{};

void precompute_zobrist() {
	for (int pce_type = PieceType::PAWN; pce_type <= PieceType::KING; pce_type++) {
		for (int col = Color::WHITE; col <= Color::BLACK; col++) {
			for (int sq = 0; sq < 64; sq++) {
				keys_pce[build_pce(static_cast<PieceType>(pce_type), static_cast<Color>(col))][sq] = random_uint64_t();
			}
		}
	}
	
	for (int sq = 0; sq < 64; sq++) {
		keys_pce[Piece::NONE][sq] = random_uint64_t();
	}

	for (int castling_rights = 0; castling_rights < 16; castling_rights++) {
		keys_castling_rights[castling_rights] = random_uint64_t();
	}

	key_side_to_move = random_uint64_t();
}

uint64_t get_zobrist_key(Position& pos) {
	uint64_t zobrist_key = 0;
	for (int sq = 0; sq < 64; sq++) {
		const Piece pce = pos.pces[sq];
		if (pce != Piece::NONE) {
			zobrist_key ^= keys_pce[static_cast<int>(pce)][sq];
		}
	}

	if (pos.en_passant_sq != Square::NO_SQ) {
		zobrist_key ^= keys_pce[static_cast<int>(Piece::NONE)][static_cast<int>(pos.en_passant_sq)];
	}
		
	zobrist_key ^= keys_castling_rights[pos.castling_rights];

	if (pos.side_to_move == Color::BLACK) {
		zobrist_key ^= key_side_to_move;
	}

	return zobrist_key;
}