#pragma once

#include "types.h"
#include <cstdint>
#include <string>

enum class MoveFlag : uint32_t {
	NONE	   = 0b00000000000000000000000,
	PAWN_START = 0b00100000000000000000000,
	CASTLE	   = 0b01000000000000000000000,
	EN_PASSANT = 0b10000000000000000000000
};

std::string get_move_str(uint32_t move);
constexpr uint32_t build_move(int from_sq, int to_sq, Piece capture_pce, Piece promo_pce, MoveFlag flag) {
	return from_sq | (to_sq << 6) | (static_cast<uint32_t>(capture_pce) << 12) | (static_cast<uint32_t>(promo_pce) << 16) | (static_cast<uint32_t>(flag));
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