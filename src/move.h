#pragma once

#include "types.h"
#include "board.h"
#include <cstdint>
#include <string>

constexpr uint32_t null_move_val = 0;

enum class MoveFlag : uint32_t {
	NONE	   = 0b00000000000000000000000,
	PAWN_START = 0b00100000000000000000000,
	CASTLE	   = 0b01000000000000000000000,
	EN_PASSANT = 0b10000000000000000000000
};

struct Move {
	uint32_t val;

	inline Move() : val(null_move_val) {}

	inline Move(uint32_t move_val) {
		val = move_val;
	}

	inline Move(uint32_t from_sq, uint32_t to_sq, Piece capture_pce, Piece promo_pce, MoveFlag flag) {
		val = (from_sq | (to_sq << 6) | (static_cast<uint32_t>(capture_pce) << 12) | (static_cast<uint32_t>(promo_pce) << 16) | (static_cast<uint32_t>(flag)));
	}

	constexpr bool operator == (Move& move) {
		return val == move.val;
	}

	constexpr bool operator != (Move& move) {
		return val != move.val;
	}

	constexpr uint32_t get_from_sq() {
		return val & 0b111111;
	}

	constexpr uint32_t get_to_sq() {
		return (val >> 6) & 0b111111;
	}

	constexpr Piece get_cap_pce() {
		return static_cast<Piece>((val >> 12) & 0b1111);
	}

	constexpr Piece get_promo_pce() {
		return static_cast<Piece>((val >> 16) & 0b1111);
	}

	constexpr bool is_pawn_start() {
		return val & static_cast<uint32_t>(MoveFlag::PAWN_START);
	}

	constexpr bool is_castle() {
		return val & static_cast<uint32_t>(MoveFlag::CASTLE);
	}

	constexpr bool is_en_passant() {
		return val & static_cast<uint32_t>(MoveFlag::EN_PASSANT);
	}

	inline std::string Move::to_str() {
		uint32_t from_sq = get_from_sq();
		int from_rank = get_rank(from_sq);
		int from_file = get_file(from_sq);
		uint32_t to_sq = get_to_sq();
		int to_rank = get_rank(to_sq);
		int to_file = get_file(to_sq);

		std::string move_str;
		move_str.push_back('a' + from_file);
		move_str.push_back('1' + from_rank);
		move_str.push_back('a' + to_file);
		move_str.push_back('1' + to_rank);

		Piece promo_pce = get_promo_pce();

		if (static_cast<uint32_t>(promo_pce)) {
			move_str.push_back(tolower(pce_to_symbol(promo_pce)));
		}

		return move_str;
	}
};