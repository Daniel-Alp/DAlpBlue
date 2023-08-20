#include "attacks.h"
#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "makemove.h"
#include <array>

void clr_pce(Position& pos, int sq) {
	Piece pce = pos.pces[sq];
	Color col = get_col(pce);
	uint64_t sq_bb = get_sq_bitboard(sq);

	pos.pces[sq] = Piece::NONE;
	pos.pce_bitboards[static_cast<uint32_t>(pce)] = clr_sq(pos.pce_bitboards[static_cast<uint32_t>(pce)], sq_bb);
	pos.col_bitboards[static_cast<uint32_t>(col)] = clr_sq(pos.col_bitboards[static_cast<uint32_t>(col)], sq_bb);
	pos.all_bitboard = clr_sq(pos.all_bitboard, sq_bb);
}

void add_pce(Position& pos, Piece& pce, int sq) {
	pos.pces[sq] = pce;
	Color col = get_col(pce);
	uint64_t sq_bb = get_sq_bitboard(sq);

	pos.pces[sq] = pce;
	pos.pce_bitboards[static_cast<uint32_t>(pce)] = set_sq(pos.pce_bitboards[static_cast<uint32_t>(pce)], sq_bb);
	pos.col_bitboards[static_cast<uint32_t>(col)] = set_sq(pos.col_bitboards[static_cast<uint32_t>(col)], sq_bb);
	pos.all_bitboard = set_sq(pos.all_bitboard, sq_bb);
}

void move_pce(Position& pos, int from_sq, int to_sq) {
	Piece pce = pos.pces[from_sq];
	Color col = get_col(pce);
	uint64_t from_sq_bb = get_sq_bitboard(from_sq);
	uint64_t to_sq_bb = get_sq_bitboard(to_sq);

	pos.pces[from_sq] = Piece::NONE;
	pos.pces[to_sq] = pce;
	pos.pce_bitboards[static_cast<uint32_t>(pce)] = move_sq(pos.pce_bitboards[static_cast<uint32_t>(pce)], from_sq_bb, to_sq_bb);
	pos.col_bitboards[static_cast<uint32_t>(col)] = move_sq(pos.col_bitboards[static_cast<uint32_t>(col)], from_sq_bb, to_sq_bb);
	pos.all_bitboard = move_sq(pos.all_bitboard, from_sq_bb, to_sq_bb);
}

bool make_move(Position& pos, uint32_t move) {
	pos.undo_stack[pos.ply].en_passant_sq = pos.en_passant_sq;
	pos.undo_stack[pos.ply].castling_rights = pos.castling_rights;
	pos.undo_stack[pos.ply].fifty_move_rule = pos.fifty_move_rule;
	pos.undo_stack[pos.ply].zobrist_key = pos.zobrist_key;

	uint32_t from_sq = get_move_from_sq(move);
	uint32_t to_sq = get_move_to_sq(move);
	
	if (pos.en_passant_sq != static_cast<uint32_t>(Square::NO_SQ)) {
		pos.en_passant_sq = static_cast<uint32_t>(Square::NO_SQ);
	}
	pos.castling_rights &= castling[from_sq];
	pos.castling_rights &= castling[to_sq];

	pos.fifty_move_rule++;

	if (move & static_cast<uint32_t>(MoveFlag::CASTLE)) {
		move_pce(pos, from_sq, to_sq);
		switch (to_sq) {
		case static_cast<uint32_t>(Square::G1):
			move_pce(pos, static_cast<uint32_t>(Square::H1), static_cast<uint32_t>(Square::F1));
			break;
		case static_cast<uint32_t>(Square::C1):
			move_pce(pos, static_cast<uint32_t>(Square::A1), static_cast<uint32_t>(Square::D1));
			break;
		case static_cast<uint32_t>(Square::G8):
			move_pce(pos, static_cast<uint32_t>(Square::H8), static_cast<uint32_t>(Square::F8));
			break;
		case static_cast<uint32_t>(Square::C8):
			move_pce(pos, static_cast<uint32_t>(Square::A8), static_cast<uint32_t>(Square::D8));
			break;
		}
	}
	else {
		Piece capture_pce = get_move_cap_pce(move);
		if (static_cast<uint32_t>(capture_pce)) {
			pos.fifty_move_rule = 0;
			if (move & static_cast<uint32_t>(MoveFlag::EN_PASSANT)) {
				clr_pce(pos, to_sq ^ 8);
			}
			else {
				clr_pce(pos, to_sq);
			}
		}
		move_pce(pos, from_sq, to_sq);

		if (get_pce_type(pos.pces[to_sq]) == PieceType::PAWN) {
			pos.fifty_move_rule = 0;
			Piece promo_pce = get_move_promo_pce(move);
			if (static_cast<uint32_t>(promo_pce)) {
				clr_pce(pos, to_sq);
				add_pce(pos, promo_pce, to_sq);
			}
			else if (move & static_cast<uint32_t>(MoveFlag::PAWN_START)) {
				pos.en_passant_sq = to_sq ^ 8;
			}
		}
	}

	Color move_col = pos.side_to_move;
	pos.side_to_move = flip_col(pos.side_to_move);

	pos.history_ply++;
	pos.ply++;

	if (sq_attacked(pos, get_lsb(pos.pce_bitboards[static_cast<uint32_t>(build_pce(PieceType::KING, move_col))]), pos.side_to_move)) {
		undo_move(pos, move);
		return 0;
	}

	return 1;
}

void undo_move(Position& pos, uint32_t move) {
	pos.history_ply--;
	pos.ply--;
	pos.side_to_move = flip_col(pos.side_to_move);

	uint32_t from_sq = get_move_from_sq(move);
	uint32_t to_sq = get_move_to_sq(move);

	if (move & static_cast<uint32_t>(MoveFlag::CASTLE)) {
		move_pce(pos, to_sq, from_sq);
		switch (to_sq) {
		case static_cast<uint32_t>(Square::G1):
			move_pce(pos, static_cast<uint32_t>(Square::F1), static_cast<uint32_t>(Square::H1));
			break;
		case static_cast<uint32_t>(Square::C1):
			move_pce(pos, static_cast<uint32_t>(Square::D1), static_cast<uint32_t>(Square::A1));
			break;
		case static_cast<uint32_t>(Square::G8):
			move_pce(pos, static_cast<uint32_t>(Square::F8), static_cast<uint32_t>(Square::H8));
			break;
		case static_cast<uint32_t>(Square::C8):
			move_pce(pos, static_cast<uint32_t>(Square::D8), static_cast<uint32_t>(Square::A8));
			break;
		}
	}
	else {
		Piece promo_pce = get_move_promo_pce(move);
		if (static_cast<uint32_t>(promo_pce)) {
			clr_pce(pos, to_sq);
			Piece undo_promo_pce = build_pce(PieceType::PAWN, pos.side_to_move);
			add_pce(pos, undo_promo_pce, to_sq);
		}

		move_pce(pos, to_sq, from_sq);

		Piece capture_pce = get_move_cap_pce(move);

		if (static_cast<uint32_t>(capture_pce)) {
			if (move & static_cast<uint32_t>(MoveFlag::EN_PASSANT)) {
				add_pce(pos, capture_pce, to_sq ^ 8);
			}
			else {
				add_pce(pos, capture_pce, to_sq);
			}
		}
	}

	pos.en_passant_sq = pos.undo_stack[pos.ply].en_passant_sq;
	pos.castling_rights = pos.undo_stack[pos.ply].castling_rights;
	pos.fifty_move_rule = pos.undo_stack[pos.ply].fifty_move_rule;
	pos.zobrist_key = pos.undo_stack[pos.ply].zobrist_key;
}