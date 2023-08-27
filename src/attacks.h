#pragma once

#include "board.h"
#include "bitboard.h"
#include "types.h"
#include <array>
#include <cstdint>

bool sq_attacked(Position& pos, int sq, Color side_attacking);
void print_attacks(Position& pos, Color side_attacking);
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