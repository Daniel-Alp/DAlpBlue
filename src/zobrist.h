#pragma once

#include "board.h"
#include <array>
#include <cstdint>

inline uint64_t seed = UINT64_C(1070372);

extern uint64_t key_side_to_move;
extern std::array<std::array<uint64_t, 64>, 15> keys_pce;
extern std::array<uint64_t, 16> keys_castling_rights;

void precompute_zobrist();
uint64_t get_zobrist_key(Position& pos);

//From StockFish
constexpr uint64_t random_uint64_t() {
	seed ^= seed >> 12;
	seed ^= seed << 25;
	seed ^= seed >> 27;
	seed *= UINT64_C(2685821657736338717);
	return seed;
}
constexpr uint64_t hash_pce(uint64_t zobrist_key, Piece pce, const int sq) {
	return zobrist_key ^ keys_pce[pce][sq];
}
constexpr uint64_t hash_castling_rights(uint64_t zobrist_key, const int castling_rights) {
	return zobrist_key ^ keys_castling_rights[castling_rights];
}
constexpr uint64_t hash_side_to_move(uint64_t zobrist_key) {
	return zobrist_key ^ key_side_to_move;
}
inline uint64_t hash_en_passant_sq(uint64_t zobrist_key, Square sq) {
	return zobrist_key ^ keys_pce[Piece::NONE][sq];
}