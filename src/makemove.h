#pragma once

#include "board.h"
#include <cstdint>

constexpr std::array<uint32_t, 64> castling = {
	13, 15, 15, 15, 12, 15, 15, 14,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15,
	7,  15, 15, 15,  3, 15, 15, 11
};

void clr_pce(Position& pos, int sq);
void add_pce(Position& pos, Piece pce, int sq);
void move_pce(Position& pos, int from_sq, int to_sq);
bool make_move(Position& pos, uint32_t move);
void undo_move(Position& pos, uint32_t move);