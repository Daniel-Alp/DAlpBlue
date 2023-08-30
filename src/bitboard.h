#pragma once

#include <array>
#include <cstdint>
#include <intrin.h>

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
void print_bitboard(uint64_t bitboard);

constexpr uint64_t shift_nort(uint64_t bitboard) {
	return bitboard << 8;
}
constexpr uint64_t shift_noea(uint64_t bitboard) {
	return (bitboard << 9) & 0xfefefefefefefefe;
}
constexpr uint64_t shift_east(uint64_t bitboard) {
	return (bitboard << 1) & 0xfefefefefefefefe;
}
constexpr uint64_t shift_soea(uint64_t bitboard) {
	return (bitboard >> 7) & 0xfefefefefefefefe;
}
constexpr uint64_t shift_sout(uint64_t bitboard) {
	return bitboard >> 8;
}
constexpr uint64_t shift_sowe(uint64_t bitboard) {
	return (bitboard >> 9) & 0x7f7f7f7f7f7f7f7f;
}
constexpr uint64_t shift_west(uint64_t bitboard) {
	return (bitboard >> 1) & 0x7f7f7f7f7f7f7f7f;
}
constexpr uint64_t shift_nowe(uint64_t bitboard) {
	return (bitboard << 7) & 0x7f7f7f7f7f7f7f7f;
}
constexpr uint64_t get_sq_bitboard(uint32_t sq) {
	return UINT64_C(1) << sq;
}
constexpr bool has_sq(uint64_t bitboard, uint64_t sq) {
	return bitboard & sq;
}
constexpr uint64_t set_sq(uint64_t bitboard, uint64_t sq) {
	return bitboard | sq;
}
constexpr uint64_t clr_sq(uint64_t bitboard, uint64_t sq) {
	return bitboard ^ sq;
}
constexpr uint64_t move_sq(uint64_t bitboard, uint64_t from_sq, uint64_t to_sq) {
	return bitboard ^ from_sq ^ to_sq;
}
inline int get_lsb(uint64_t bitboard) {
	unsigned long i = 0;
	_BitScanForward64(&i, bitboard);
	return i;
}
inline int get_msb(uint64_t bitboard) {
	unsigned long i = 0;
	_BitScanReverse64(&i, bitboard);
	return i;
}
constexpr uint64_t clr_lsb(uint64_t bitboard) {
	return bitboard & (bitboard - 1);
}