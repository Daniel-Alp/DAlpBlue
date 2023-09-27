#pragma once

#include "types.h"
#include "board.h"
#include <array>
#include <cstdint>
#include <string>

constexpr uint32_t null_move_val = 0;

enum MoveFlag : uint32_t {
	NO_FLAG	   = 0b00000000000000000000000,
	PAWN_START = 0b00100000000000000000000,
	CASTLE	   = 0b01000000000000000000000,
	EN_PASSANT = 0b10000000000000000000000
};

class Move {
private:
	uint32_t val;
public:
	inline Move() : val(null_move_val) {}

	inline Move(uint32_t from_sq, uint32_t to_sq, Piece capture_pce, Piece promo_pce, MoveFlag flag) {
		val = (from_sq | (to_sq << 6) | (capture_pce << 12) | (promo_pce << 16) | flag);
	}

	constexpr bool operator == (const Move& move) const {
		return val == move.val;
	}

	constexpr bool operator != (const Move& move) const {
		return val != move.val;
	}

	constexpr uint32_t get_from_sq() const {
		return val & 0b111111;
	}

	constexpr uint32_t get_to_sq() const {
		return (val >> 6) & 0b111111;
	}

	constexpr Piece get_cap_pce() const {
		return static_cast<Piece>((val >> 12) & 0b1111);
	}

	constexpr Piece get_promo_pce() const {
		return static_cast<Piece>((val >> 16) & 0b1111);
	}

	constexpr bool is_pawn_start() const {
		return val & static_cast<uint32_t>(MoveFlag::PAWN_START);
	}

	constexpr bool is_castle() const {
		return val & static_cast<uint32_t>(MoveFlag::CASTLE);
	}

	constexpr bool is_en_passant() const {
		return val & static_cast<uint32_t>(MoveFlag::EN_PASSANT);
	}

	constexpr bool is_quiet() const {
		return get_cap_pce() == Piece::NONE && get_promo_pce() == Piece::NONE;
	}

	inline std::string Move::to_str() const {
		const uint32_t from_sq = get_from_sq();
		const int from_rank = get_rank(from_sq);
		const int from_file = get_file(from_sq);
		const uint32_t to_sq = get_to_sq();
		const int to_rank = get_rank(to_sq);
		const int to_file = get_file(to_sq);

		std::string move_str;
		move_str.push_back('a' + from_file);
		move_str.push_back('1' + from_rank);
		move_str.push_back('a' + to_file);
		move_str.push_back('1' + to_rank);

		const Piece promo_pce = get_promo_pce();

		if (promo_pce != Piece::NONE) { //DA!!!
			move_str.push_back(tolower(pce_to_symbol(promo_pce)));
		}

		return move_str;
	}
};

class MoveList {
public:
	static constexpr int max_moves = 218;
	inline int size() const {
		return m_size;
	}

	inline void push_back(const Move move) {
		moves[m_size++] = move;
	}

	inline Move get(const int i) const {
		return moves[i];
	}

	inline void swap(const int i1, const int i2) {
		std::swap(moves[i1], moves[i2]);
	}
private:
	int m_size = 0;
	std::array<Move, max_moves> moves{};
};