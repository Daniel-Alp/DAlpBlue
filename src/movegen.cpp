#include "board.h"
#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "types.h"
#include <array>
#include <cstdint>
#include <iostream>

MoveList gen_pseudo_moves(const Position& pos, const bool exclude_quiet) {
	MoveList move_list;

	const Piece knight = build_pce(PieceType::KNIGHT, pos.side_to_move);
	const Piece king = build_pce(PieceType::KING, pos.side_to_move);
	const Piece bishop = build_pce(PieceType::BISHOP, pos.side_to_move);
	const Piece rook = build_pce(PieceType::ROOK, pos.side_to_move);
	const Piece queen = build_pce(PieceType::QUEEN, pos.side_to_move);

	uint64_t targets = 0xffffffffffffffff;

	if (exclude_quiet) {
		targets = pos.all_bitboard;
	}
	else {
		gen_castling_moves(pos, move_list);
	}

	gen_pawn_moves(pos, move_list, targets, pos.side_to_move);
	gen_knight_moves(pos, move_list, targets, pos.pce_bitboards[static_cast<int>(knight)]);
	gen_king_moves(pos, move_list, targets, pos.pce_bitboards[static_cast<int>(king)]);
	gen_bishop_moves(pos, move_list, targets, pos.pce_bitboards[static_cast<int>(bishop)]);
	gen_rook_moves(pos, move_list, targets, pos.pce_bitboards[static_cast<int>(rook)]);
	gen_queen_moves(pos, move_list, targets, pos.pce_bitboards[static_cast<int>(queen)]);

	return move_list;
};

void serialize_moves(const Position& pos, MoveList& move_list, const uint64_t targets, uint64_t attacks, const int from_sq) {
	attacks &= ~pos.col_bitboards[static_cast<int>(pos.side_to_move)];
	attacks &= targets;
	while (attacks != 0) {
		const int to_sq = get_lsb(attacks);
		move_list.push_back(Move(from_sq, to_sq, pos.pces[to_sq], Piece::NONE, MoveFlag::NONE));
		attacks = clr_lsb(attacks);
	}
}

void serialize_pawn_promo(const Position& pos, MoveList& move_list, uint64_t to_sqs, const int dir) {
	while (to_sqs != 0) {
		const int to_sq = get_lsb(to_sqs);
		const int from_sq = to_sq - dir;
		const Piece capture_pce = pos.pces[to_sq];
		move_list.push_back(Move(from_sq, to_sq, capture_pce, build_pce(PieceType::QUEEN, pos.side_to_move), MoveFlag::NONE));
		move_list.push_back(Move(from_sq, to_sq, capture_pce, build_pce(PieceType::ROOK, pos.side_to_move), MoveFlag::NONE));
		move_list.push_back(Move(from_sq, to_sq, capture_pce, build_pce(PieceType::BISHOP, pos.side_to_move), MoveFlag::NONE));
		move_list.push_back(Move(from_sq, to_sq, capture_pce, build_pce(PieceType::KNIGHT, pos.side_to_move), MoveFlag::NONE));
		to_sqs = clr_lsb(to_sqs);
	}
}

void serialize_pawn_non_promo(const Position& pos, MoveList& move_list, uint64_t to_sqs, const int dir, const MoveFlag flag) {
	while (to_sqs != 0) {
		const int to_sq = get_lsb(to_sqs);
		move_list.push_back(Move(to_sq - dir, to_sq, pos.pces[to_sq], Piece::NONE, flag));
		to_sqs = clr_lsb(to_sqs);
	}
}

void gen_pawn_moves(const Position& pos, MoveList& move_list, const uint64_t targets, const Color col) {
	const uint64_t empty = ~pos.all_bitboard;
	if (col == Color::WHITE) {
		const uint64_t pawns = pos.pce_bitboards[static_cast<int>(Piece::WHITE_PAWN)];
		const uint64_t enemy = pos.col_bitboards[static_cast<int>(Color::BLACK)];

		const uint64_t single_push = shift_nort(pawns) & empty;
		serialize_pawn_promo(pos, move_list, single_push & rank_8, 8);
		serialize_pawn_non_promo(pos, move_list, single_push & (~rank_8) & targets, 8, MoveFlag::NONE);

		const uint64_t double_push = shift_nort(single_push) & empty & rank_4 & targets;
		serialize_pawn_non_promo(pos, move_list, double_push, 16, MoveFlag::PAWN_START);

		const uint64_t capture_noea = shift_noea(pawns) & enemy;
		serialize_pawn_promo(pos, move_list, capture_noea & rank_8, 9);
		serialize_pawn_non_promo(pos, move_list, capture_noea & (~rank_8), 9, MoveFlag::NONE);

		const uint64_t capture_nowe = shift_nowe(pawns) & enemy;
		serialize_pawn_promo(pos, move_list, capture_nowe & rank_8, 7);
		serialize_pawn_non_promo(pos, move_list, capture_nowe & (~rank_8), 7, MoveFlag::NONE);

		if (pos.en_passant_sq != static_cast<int>(Square::NO_SQ)) {
			uint64_t capture_en_passant = black_pawn_attacks[pos.en_passant_sq] & pawns;
			while (capture_en_passant != 0) {
				const int from_sq = get_lsb(capture_en_passant);
				move_list.push_back(Move(from_sq, pos.en_passant_sq, Piece::BLACK_PAWN, Piece::NONE, MoveFlag::EN_PASSANT));
				capture_en_passant = clr_lsb(capture_en_passant);
			}
		}
	}
	else {
		const uint64_t pawns = pos.pce_bitboards[static_cast<int>(Piece::BLACK_PAWN)];
		const uint64_t enemy = pos.col_bitboards[static_cast<int>(Color::WHITE)];

		const uint64_t single_push = shift_sout(pawns) & empty;
		serialize_pawn_promo(pos, move_list, single_push & rank_1, -8);
		serialize_pawn_non_promo(pos, move_list, single_push & (~rank_1) & targets, -8, MoveFlag::NONE);

		const uint64_t double_push = shift_sout(single_push) & empty & rank_5 & targets;
		serialize_pawn_non_promo(pos, move_list, double_push, -16, MoveFlag::PAWN_START);

		const uint64_t capture_sowe = shift_sowe(pawns) & enemy;
		serialize_pawn_promo(pos, move_list, capture_sowe & rank_1, -9);
		serialize_pawn_non_promo(pos, move_list, capture_sowe & (~rank_1), -9, MoveFlag::NONE);

		const uint64_t capture_soea = shift_soea(pawns) & enemy;
		serialize_pawn_promo(pos, move_list, capture_soea & rank_1, -7);
		serialize_pawn_non_promo(pos, move_list, capture_soea & (~rank_1), -7, MoveFlag::NONE);

		if (pos.en_passant_sq != static_cast<int>(Square::NO_SQ)) {
			uint64_t capture_en_passant = white_pawn_attacks[pos.en_passant_sq] & pawns;
			while (capture_en_passant != 0) {
				const int from_sq = get_lsb(capture_en_passant);
				move_list.push_back(Move(from_sq, pos.en_passant_sq, Piece::WHITE_PAWN, Piece::NONE, MoveFlag::EN_PASSANT));
				capture_en_passant = clr_lsb(capture_en_passant);
			}
		}
	}
}

void gen_king_moves(const Position& pos, MoveList& move_list, const uint64_t targets, uint64_t king) {
	const int from_sq = get_lsb(king);
	const uint64_t attacks = king_attacks[from_sq];
	serialize_moves(pos, move_list, targets, attacks, from_sq);
}

void gen_knight_moves(const Position& pos, MoveList& move_list, const uint64_t targets, uint64_t knights) {
	while (knights != 0) {
		const int from_sq = get_lsb(knights);
		const uint64_t attacks = knight_attacks[from_sq];
		serialize_moves(pos, move_list, targets, attacks, from_sq);
		knights = clr_lsb(knights);
	}
}

void gen_bishop_moves(const Position& pos, MoveList& move_list, const uint64_t targets, uint64_t bishops) {
	while (bishops != 0) {
		const int from_sq = get_lsb(bishops);
		const uint64_t attacks = gen_bishop_attacks(from_sq, pos.all_bitboard);
		serialize_moves(pos, move_list, targets, attacks, from_sq);
		bishops = clr_lsb(bishops);
	}
};

void gen_rook_moves(const Position& pos, MoveList& move_list, const uint64_t targets, uint64_t rooks) {
	while (rooks != 0) {
		const int from_sq = get_lsb(rooks);
		const uint64_t attacks = gen_rook_attacks(from_sq, pos.all_bitboard);
		serialize_moves(pos, move_list, targets, attacks, from_sq);
		rooks = clr_lsb(rooks);
	}
};

void gen_queen_moves(const Position& pos, MoveList& move_list, const uint64_t targets, uint64_t queens) {
	while (queens != 0) {
		const int from_sq = get_lsb(queens);
		const uint64_t attacks = gen_rook_attacks(from_sq, pos.all_bitboard) ^ gen_bishop_attacks(from_sq, pos.all_bitboard);
		serialize_moves(pos, move_list, targets, attacks, from_sq);
		queens = clr_lsb(queens);
	}
};

void gen_castling_moves(const Position& pos, MoveList& move_list) {
	if (pos.side_to_move == Color::WHITE) {
		if (sq_attacked(pos, static_cast<int>(Square::E1), Color::BLACK)) {
			return;
		}
		if ((pos.castling_rights & static_cast<int>(CastlingRights::WHITE_SHORT)) &&
			!(pos.all_bitboard & sq_between_e1_h1) &&
			!sq_attacked(pos, static_cast<int>(Square::F1), Color::BLACK)) {
			move_list.push_back(Move(static_cast<int>(Square::E1), static_cast<int>(Square::G1), Piece::NONE, Piece::NONE, MoveFlag::CASTLE));
		}
		if ((pos.castling_rights & static_cast<int>(CastlingRights::WHITE_LONG)) &&
			!(pos.all_bitboard & sq_between_e1_a1) &&
			!sq_attacked(pos, static_cast<int>(Square::D1), Color::BLACK)) {
			move_list.push_back(Move(static_cast<int>(Square::E1), static_cast<int>(Square::C1), Piece::NONE, Piece::NONE, MoveFlag::CASTLE));
		}
	}
	else {
		if (sq_attacked(pos, static_cast<int>(Square::E8), Color::WHITE)) {
			return;
		}
		if ((pos.castling_rights & static_cast<int>(CastlingRights::BLACK_SHORT)) &&
			!(pos.all_bitboard & sq_between_e8_h8) &&
			!sq_attacked(pos, static_cast<int>(Square::F8), Color::WHITE)) {
			move_list.push_back(Move(static_cast<int>(Square::E8), static_cast<int>(Square::G8), Piece::NONE, Piece::NONE, MoveFlag::CASTLE));
		}
		if ((pos.castling_rights & static_cast<int>(CastlingRights::BLACK_LONG)) &&
			!(pos.all_bitboard & sq_between_e8_a8) &&
			!sq_attacked(pos, static_cast<int>(Square::D8), Color::WHITE)) {
			move_list.push_back(Move(static_cast<int>(Square::E8), static_cast<int>(Square::C8), Piece::NONE, Piece::NONE, MoveFlag::CASTLE));
		}
	}
}