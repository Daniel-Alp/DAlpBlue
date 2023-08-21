#pragma once

#include "types.h"
#include <cstdint>
#include <array>
#include <string>

extern const std::string start_fen;

enum Square : uint32_t {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

struct Undo {
	uint32_t en_passant_sq;
	uint32_t castling_rights;
	uint32_t fifty_move_rule;
	uint64_t zobrist_key;
};

struct Position {
	Color side_to_move;

	std::array<Piece, 64> pces;
	std::array<uint64_t, 15> pce_bitboards;
	std::array<uint64_t, 2> col_bitboards;
	uint64_t all_bitboard;

	uint32_t castling_rights;
	uint32_t en_passant_sq;
	uint32_t fifty_move_rule;
	uint64_t zobrist_key;

	uint32_t ply;
	uint64_t history_ply;
	std::array<Undo, 256> undo_stack;
	std::array<uint64_t, 256> history_stack;
};

void load_from_fen(Position& pos, std::string& fen);
void print_board(Position& pos);
constexpr uint32_t get_rank(uint32_t sq) {
	return sq >> 3;
}
constexpr uint32_t get_file(uint32_t sq) {
	return sq & 7;
}
constexpr uint32_t get_sq(uint32_t rank, uint32_t file) {
	return (rank << 3) + file;
}
constexpr uint32_t mirror_sq(uint32_t sq) {
	return sq ^ 56;
}
constexpr Color flip_col(Color& col) {
	return static_cast<Color>(static_cast<uint32_t>(col) ^ 1);
}
