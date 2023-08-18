#pragma once

#include "types.h"
#include <cstdint>
#include <string>

constexpr int max_moves = 218;
extern uint32_t flag_none;
extern uint32_t flag_pawn_start;
extern uint32_t flag_castle;
extern uint32_t flag_en_passant;

std::string get_move_str(uint32_t move);
void print_moves(std::array<uint32_t, 218>& moves, int num_moves);
constexpr uint32_t build_move(uint32_t from_sq, uint32_t to_sq, Piece capture_pce, Piece promo_pce, uint32_t flags) {
	return from_sq | (to_sq << 6) | (static_cast<uint32_t>(capture_pce) << 12) | (static_cast<uint32_t>(promo_pce) << 16) | flags;
}
constexpr uint32_t move_from_sq(uint32_t move) {
	return move & 0b111111;
}
constexpr uint32_t move_to_sq(uint32_t move) {
	return (move >> 6) & 0b111111;
}
constexpr Piece move_capture_pce(uint32_t move) {
	return static_cast<Piece>((move >> 12) & 0b1111);
}
constexpr Piece move_promo_pce(uint32_t move) {
	return static_cast<Piece>((move >> 16) & 0b1111);
}