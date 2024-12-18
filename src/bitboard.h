#pragma once

#include <array>
#include <cstdint>
#include <x86intrin.h>

extern std::array<uint64_t, 64> ray_nort;
extern std::array<uint64_t, 64> ray_noea;
extern std::array<uint64_t, 64> ray_east;
extern std::array<uint64_t, 64> ray_soea;
extern std::array<uint64_t, 64> ray_sout;
extern std::array<uint64_t, 64> ray_sowe;
extern std::array<uint64_t, 64> ray_west;
extern std::array<uint64_t, 64> ray_nowe;

extern std::array<uint64_t, 64> ray_nort_stop;
extern std::array<uint64_t, 64> ray_noea_stop;
extern std::array<uint64_t, 64> ray_east_stop;
extern std::array<uint64_t, 64> ray_soea_stop;
extern std::array<uint64_t, 64> ray_sout_stop;
extern std::array<uint64_t, 64> ray_sowe_stop;
extern std::array<uint64_t, 64> ray_west_stop;
extern std::array<uint64_t, 64> ray_nowe_stop;

extern std::array<uint64_t, 64> ray_bishop;
extern std::array<uint64_t, 64> ray_rook;
extern std::array<uint64_t, 64> ray_queen;

extern std::array<uint64_t, 64> white_pawn_attacks;
extern std::array<uint64_t, 64> black_pawn_attacks;
extern std::array<uint64_t, 64> knight_attacks;
extern std::array<uint64_t, 64> king_attacks;

constexpr inline uint64_t rank_1 = 0x00000000000000FF;
constexpr inline uint64_t rank_4 = 0x00000000FF000000;
constexpr inline uint64_t rank_5 = 0x000000FF00000000;
constexpr inline uint64_t rank_8 = 0xFF00000000000000;

constexpr inline uint64_t sq_between_e1_h1 = 0x0000000000000060;
constexpr inline uint64_t sq_between_e1_a1 = 0x000000000000000E;
constexpr inline uint64_t sq_between_e8_h8 = 0x6000000000000000;
constexpr inline uint64_t sq_between_e8_a8 = 0x0E00000000000000;

void precompute_rays();
void precompute_non_slider_attacks();

inline uint64_t shift_nort(const uint64_t bitboard) {
	return bitboard << 8;
}
inline uint64_t shift_noea(const uint64_t bitboard) {
	return (bitboard << 9) & 0xfefefefefefefefe;
}
inline uint64_t shift_east(const uint64_t bitboard) {
	return (bitboard << 1) & 0xfefefefefefefefe;
}
inline uint64_t shift_soea(const uint64_t bitboard) {
	return (bitboard >> 7) & 0xfefefefefefefefe;
}
inline uint64_t shift_sout(const uint64_t bitboard) {
	return bitboard >> 8;
}
inline uint64_t shift_sowe(const uint64_t bitboard) {
	return (bitboard >> 9) & 0x7f7f7f7f7f7f7f7f;
}
inline uint64_t shift_west(const uint64_t bitboard) {
	return (bitboard >> 1) & 0x7f7f7f7f7f7f7f7f;
}
inline uint64_t shift_nowe(const uint64_t bitboard) {
	return (bitboard << 7) & 0x7f7f7f7f7f7f7f7f;
}
inline uint64_t get_sq_bitboard(const uint32_t sq) {
	return UINT64_C(1) << sq;
}
inline bool has_sq(const uint64_t bitboard, const uint64_t sq) {
	return bitboard & sq;
}
inline uint64_t set_sq(const uint64_t bitboard, const uint64_t sq) {
	return bitboard | sq;
}
inline uint64_t clear_sq(const uint64_t bitboard, const uint64_t sq) {
	return bitboard ^ sq;
}
inline uint64_t move_sq(const uint64_t bitboard, const uint64_t from_sq, const uint64_t to_sq) {
	return bitboard ^ from_sq ^ to_sq;
}
inline int get_lsb(const uint64_t bitboard) {
	return __builtin_ctzl(bitboard);
}
inline int get_msb(const uint64_t bitboard) {
	return __builtin_clzl(bitboard) ^ 63;
}
inline uint64_t clear_lsb(const uint64_t bitboard) {
	return bitboard & (bitboard - 1);
}
void print_bitboard(uint64_t bitboard);