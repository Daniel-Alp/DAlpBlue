#include "board.h"
#include "bitboard.h"
#include "movegen.h"
#include "types.h"

#include <array>
#include <cstdint>
#include <iostream>

void gen_pseudo_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, bool exclude_quiet) {
	num_moves = 0;
	Piece knight = build_pce(PieceType::KNIGHT, pos.side_to_move);
	Piece king = build_pce(PieceType::KING, pos.side_to_move);
	Piece bishop = build_pce(PieceType::BISHOP, pos.side_to_move);
	Piece rook = build_pce(PieceType::ROOK, pos.side_to_move);
	Piece queen = build_pce(PieceType::QUEEN, pos.side_to_move);

	uint64_t targets = 0xffffffffffffffff;

	if (exclude_quiet) {
		targets = pos.all_bitboard;
	}

	gen_pawn_moves(pos, moves, num_moves, targets, pos.side_to_move);
	/*gen_knight_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(knight)]);
	gen_king_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(king)]);
	gen_bishop_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(bishop)]);
	gen_rook_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(rook)]);
	gen_queen_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(queen)]);*/
};

void serialize_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t attacks, int from_sq) {
	attacks &= ~pos.col_bitboards[static_cast<uint32_t>(pos.side_to_move)];
	attacks &= targets;
	while (attacks != 0) {
		int to_sq = get_lsb(attacks);
		moves[num_moves++] = build_move(from_sq, to_sq, pos.pces[to_sq], Piece::NONE, MoveFlag::NONE);
		attacks = clr_lsb(attacks);
	}
}

void serialize_pawn_promo(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t to_sqs, int dir) { //ADD TO .H FILE
	while (to_sqs != 0) {
		int to_sq = get_lsb(to_sqs);
		int from_sq = to_sq - dir;
		Piece capture_pce = pos.pces[to_sq];
		moves[num_moves++] = build_move(from_sq, to_sq, capture_pce, build_pce(PieceType::KNIGHT, pos.side_to_move), MoveFlag::NONE);
		moves[num_moves++] = build_move(from_sq, to_sq, capture_pce, build_pce(PieceType::BISHOP, pos.side_to_move), MoveFlag::NONE);
		moves[num_moves++] = build_move(from_sq, to_sq, capture_pce, build_pce(PieceType::ROOK, pos.side_to_move), MoveFlag::NONE);
		moves[num_moves++] = build_move(from_sq, to_sq, capture_pce, build_pce(PieceType::QUEEN, pos.side_to_move), MoveFlag::NONE);
		to_sqs = clr_lsb(to_sqs);
	}
}

void serialize_pawn_non_promo(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t to_sqs, int dir, MoveFlag flag) { //ADD TO .H FILE
	while (to_sqs != 0) {
		int to_sq = get_lsb(to_sqs);
		moves[num_moves++] = build_move(to_sq - dir, to_sq, pos.pces[to_sq], Piece::NONE, flag);
		to_sqs = clr_lsb(to_sqs);
	}
}

void gen_pawn_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, Color col) {
	const uint64_t empty = ~pos.all_bitboard;
	if (col == Color::WHITE) {
		const uint64_t pawns = pos.pce_bitboards[static_cast<uint32_t>(Piece::WHITE_PAWN)];
		const uint64_t enemy = pos.col_bitboards[static_cast<uint32_t>(Color::BLACK)];

		const uint64_t single_push = shift_nort(pawns) & empty;
		serialize_pawn_promo(pos, moves, num_moves, single_push & rank_8, 8);
		serialize_pawn_non_promo(pos, moves, num_moves, single_push & (~rank_8) & targets, 8, MoveFlag::NONE);

		const uint64_t double_push = shift_nort(single_push) & empty & rank_4 & targets;
		serialize_pawn_non_promo(pos, moves, num_moves, double_push, 16, MoveFlag::PAWN_START);

		const uint64_t capture_noea = shift_noea(pawns) & enemy;
		serialize_pawn_promo(pos, moves, num_moves, capture_noea & rank_8, 9);
		serialize_pawn_non_promo(pos, moves, num_moves, capture_noea & (~rank_8), 9, MoveFlag::NONE);

		const uint64_t capture_nowe = shift_nowe(pawns) & enemy;
		serialize_pawn_promo(pos, moves, num_moves, capture_nowe & rank_8, 7);
		serialize_pawn_non_promo(pos, moves, num_moves, capture_nowe & (~rank_8), 7, MoveFlag::NONE);

		if (pos.en_passant_sq != static_cast<uint32_t>(Square::NO_SQ)) {
			uint64_t capture_en_passant = black_pawn_attacks[pos.en_passant_sq] & pawns;
			while (capture_en_passant != 0) {
				int from_sq = get_lsb(capture_en_passant);
				moves[num_moves++] = build_move(from_sq, pos.en_passant_sq, Piece::BLACK_PAWN, Piece::NONE, MoveFlag::EN_PASSANT);
				capture_en_passant = clr_lsb(capture_en_passant);
			}
		}
	}
	else { //FINISH IMPLEMENTING
		const uint64_t pawns = pos.pce_bitboards[static_cast<uint32_t>(Piece::BLACK_PAWN)];
		const uint64_t enemy = pos.col_bitboards[static_cast<uint32_t>(Color::WHITE)];

		const uint64_t single_push = shift_sout(pawns) & empty;
		serialize_pawn_promo(pos, moves, num_moves, single_push & rank_1, -8);
		serialize_pawn_non_promo(pos, moves, num_moves, single_push & (~rank_1) & targets, -8, MoveFlag::NONE);

		const uint64_t double_push = shift_sout(single_push) & empty & rank_5 & targets;
		serialize_pawn_non_promo(pos, moves, num_moves, double_push, -16, MoveFlag::PAWN_START);

		const uint64_t capture_sowe = shift_sowe(pawns) & enemy;
		serialize_pawn_promo(pos, moves, num_moves, capture_sowe & rank_1, -9);
		serialize_pawn_non_promo(pos, moves, num_moves, capture_sowe & (~rank_1), -9, MoveFlag::NONE);

		const uint64_t capture_soea = shift_soea(pawns) & enemy;
		serialize_pawn_promo(pos, moves, num_moves, capture_soea & rank_1, -7);
		serialize_pawn_non_promo(pos, moves, num_moves, capture_soea & (~rank_1), -7, MoveFlag::NONE);

		if (pos.en_passant_sq != static_cast<uint32_t>(Square::NO_SQ)) {
			uint64_t capture_en_passant = white_pawn_attacks[pos.en_passant_sq] & pawns;
			while (capture_en_passant != 0) {
				int from_sq = get_lsb(capture_en_passant);
				moves[num_moves++] = build_move(from_sq, pos.en_passant_sq, Piece::WHITE_PAWN, Piece::NONE, MoveFlag::EN_PASSANT);
				capture_en_passant = clr_lsb(capture_en_passant);
			}
		}
	}
}

void gen_king_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t king) {
	int from_sq = get_lsb(king);
	uint64_t attacks = king_attacks[from_sq];
	serialize_moves(pos, moves, num_moves, targets, attacks, from_sq);
}

void gen_knight_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t knights) {
	while (knights != 0) {
		int from_sq = get_lsb(knights);
		uint64_t attacks = knight_attacks[from_sq];
		serialize_moves(pos, moves, num_moves, targets, attacks, from_sq);
		knights = clr_lsb(knights);
	}
}

void gen_bishop_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops) {
	while (bishops != 0) {
		int from_sq = get_lsb(bishops);
		uint64_t attacks = gen_bishop_attacks(from_sq, pos.all_bitboard);
		serialize_moves(pos, moves, num_moves, targets, attacks, from_sq);
		bishops = clr_lsb(bishops);
	}
};

void gen_rook_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t rooks) {
	while (rooks != 0) {
		int from_sq = get_lsb(rooks);
		uint64_t attacks = gen_rook_attacks(from_sq, pos.all_bitboard);
		serialize_moves(pos, moves, num_moves, targets, attacks, from_sq);
		rooks = clr_lsb(rooks);
	}
};

void gen_queen_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t queens) {
	while (queens != 0) {
		int from_sq = get_lsb(queens);
		uint64_t attacks = gen_rook_attacks(from_sq, pos.all_bitboard) ^ gen_bishop_attacks(from_sq, pos.all_bitboard);
		serialize_moves(pos, moves, num_moves, targets, attacks, from_sq);
		queens = clr_lsb(queens);
	}
};

void print_moves(std::array<uint32_t, max_moves>& moves, int num_moves) {
	for (int i = 0; i < num_moves; i++) {
		std::cout << get_move_str(moves[i]) << std::endl;
	}
	std::cout << "TOTAL = " << num_moves << std::endl;
}