#pragma once

#include "bitboard.h"
#include "move.h"
#include "board.h"
#include <array>

constexpr inline int max_moves = 218;

void gen_pseudo_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, bool not_quiet);
void serialize_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t attacks, int from_sq);
void serialize_pawn_promo(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t to_sqs, int dir);
void serialize_pawn_non_promo(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t to_sqs, int dir, MoveFlag flag);
void gen_pawn_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, Color col);
void gen_king_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t king);
void gen_knight_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t knights);
void gen_bishop_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops);
void gen_rook_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops);
void gen_queen_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops);
void print_moves(std::array<uint32_t, max_moves>& moves, int num_moves);

inline uint64_t gen_bishop_attacks(int sq, uint64_t occluded) {
	occluded |= 0x8000000000000001;
	int block_noea = get_lsb(ray_noea_stop[sq] & occluded);
	int block_nowe = get_lsb(ray_nowe_stop[sq] & occluded);
	int block_soea = get_msb(ray_soea_stop[sq] & occluded);
	int block_sowe = get_msb(ray_sowe_stop[sq] & occluded);
	return ray_bishop[sq] ^ ray_noea[block_noea] ^ ray_nowe[block_nowe] ^ ray_soea[block_soea] ^ ray_sowe[block_sowe];
}

inline uint64_t gen_rook_attacks(int sq, uint64_t occluded) {
	occluded |= 0x8000000000000001;
	int block_nort = get_lsb(ray_nort_stop[sq] & occluded);
	int block_east = get_lsb(ray_east_stop[sq] & occluded);
	int block_sout = get_msb(ray_sout_stop[sq] & occluded);
	int block_west = get_msb(ray_west_stop[sq] & occluded);
	return ray_rook[sq] ^ ray_nort[block_nort] ^ ray_east[block_east] ^ ray_sout[block_sout] ^ ray_west[block_west];
}
