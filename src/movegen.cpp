#include "board.h"
#include "bitboard.h"
#include "movegen.h"
#include "move.h"
#include "types.h"

#include <array>
#include <cstdint>

void gen_pseudo_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, bool exclude_quiet) {
	num_moves = 0;
	Piece bishop = build_pce(PieceType::BISHOP, pos.side_to_move);
	Piece rook = build_pce(PieceType::ROOK, pos.side_to_move);
	Piece queen = build_pce(PieceType::QUEEN, pos.side_to_move);

	uint64_t targets = 0xffffffffffffffff;

	if (exclude_quiet) {
		targets &= pos.all_bitboard;
	}

	gen_bishop_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(bishop)]);
	gen_rook_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(rook)]);
	gen_queen_moves(pos, moves, num_moves, targets, pos.pce_bitboards[static_cast<uint32_t>(queen)]);
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