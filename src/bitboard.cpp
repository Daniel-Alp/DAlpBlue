#include "bitboard.h"
#include "board.h"
#include <iostream>

std::array<uint64_t, 64> ray_nort;
std::array<uint64_t, 64> ray_noea;
std::array<uint64_t, 64> ray_east;
std::array<uint64_t, 64> ray_soea;
std::array<uint64_t, 64> ray_sout;
std::array<uint64_t, 64> ray_sowe;
std::array<uint64_t, 64> ray_west;
std::array<uint64_t, 64> ray_nowe;

std::array<uint64_t, 64> ray_nort_stop;
std::array<uint64_t, 64> ray_noea_stop;
std::array<uint64_t, 64> ray_east_stop;
std::array<uint64_t, 64> ray_soea_stop;
std::array<uint64_t, 64> ray_sout_stop;
std::array<uint64_t, 64> ray_sowe_stop;
std::array<uint64_t, 64> ray_west_stop;
std::array<uint64_t, 64> ray_nowe_stop;

std::array<uint64_t, 64> ray_bishop;
std::array<uint64_t, 64> ray_rook;
std::array<uint64_t, 64> ray_queen;


std::array<uint64_t, 64> king_attacks{};
std::array<uint64_t, 64> knight_attacks{};

void precompute_rays() {
	uint64_t nort = 0x0101010101010100;
	for (int sq = 0; sq < 64; sq++) {
		ray_nort[sq] = nort;
		ray_nort_stop[sq] = set_sq(nort, get_sq_bitboard(63));
		nort <<= 1;
	}

	uint64_t noea = 0x8040201008040200;
	for (int file = 0; file < 8; file++) {
		uint64_t noea_2 = noea;
		for (int rank = 0; rank < 8; rank++) {
			const int sq = get_sq(rank, file);
			ray_noea[sq] = noea_2;
			ray_noea_stop[sq] = set_sq(noea_2, get_sq_bitboard(63));
			noea_2 = shift_nort(noea_2);
		}
		noea = shift_east(noea);
	}

	uint64_t east = 0xFE;
	for (int file = 0; file < 8; file++) {
		uint64_t east_2 = east;
		for (int rank = 0; rank < 8; rank++) {
			const int sq = get_sq(rank, file);
			ray_east[sq] = east_2;
			ray_east_stop[sq] = set_sq(east_2, get_sq_bitboard(63));
			east_2 = shift_nort(east_2);
		}
		east = shift_east(east);
	}

	uint64_t nowe = 0x102040810204000;
	for (int file = 7; file >= 0; file--) {
		uint64_t nowe_2 = nowe;
		for (int rank = 0; rank < 8; rank++) {
			const int sq = get_sq(rank, file);
			ray_nowe[sq] = nowe_2;
			ray_nowe_stop[sq] = set_sq(nowe_2, get_sq_bitboard(63));
			nowe_2 = shift_nort(nowe_2);
		}
		nowe = shift_west(nowe);
	}

	uint64_t sout = 0x80808080808080;
	for (int sq = 63; sq >= 0; sq--) {
		ray_sout[sq] = sout;
		ray_sout_stop[sq] = set_sq(sout, get_sq_bitboard(0));
		sout >>= 1;
	}

	uint64_t sowe = 0x40201008040201;
	for (int file = 7; file >= 0; file--) {
		uint64_t sowe_2 = sowe;
		for (int rank = 7; rank >= 0; rank--) {
			const int sq = get_sq(rank, file);
			ray_sowe[sq] = sowe_2;
			ray_sowe_stop[sq] = set_sq(sowe_2, get_sq_bitboard(0));
			sowe_2 = shift_sout(sowe_2);
		}
		sowe = shift_west(sowe);
	}

	uint64_t west = 0x7F;
	for (int file = 7; file >= 0; file--) {
		uint64_t west_2 = west;
		for (int rank = 0; rank < 8; rank++) {
			const int sq = get_sq(rank, file);
			ray_west[sq] = west_2;
			ray_west_stop[sq] = set_sq(west_2, get_sq_bitboard(0));
			west_2 = shift_nort(west_2);
		}
		west = shift_west(west);
	}

	uint64_t soea = 0x2040810204080;
	for (int file = 0; file < 8; file++) {
		uint64_t soea_2 = soea;
		for (int rank = 7; rank >= 0; rank--) {
			const int sq = get_sq(rank, file);
			ray_soea[sq] = soea_2;
			ray_soea_stop[sq] = set_sq(soea_2, get_sq_bitboard(0));
			soea_2 = shift_sout(soea_2);
		}
		soea = shift_east(soea);
	}

	for (int sq = 0; sq < 64; sq++) {
		ray_bishop[sq] = ray_noea[sq] ^ ray_soea[sq] ^ ray_sowe[sq] ^ ray_nowe[sq];
		ray_rook[sq] = ray_nort[sq] ^ ray_east[sq] ^ ray_sout[sq] ^ ray_west[sq];
		ray_queen[sq] = ray_bishop[sq] ^ ray_rook[sq];
	}
}

void precompute_non_slider_attacks() {
	uint64_t king = 1;
	for (int sq = 0; sq < 64; sq++) {
		uint64_t attacks = shift_east(king) | shift_west(king) | king;
		attacks = (attacks | shift_nort(attacks) | shift_sout(attacks)) ^ king;
		king_attacks[sq] = attacks;
		king <<= 1;
	}

	uint64_t knight = 1;
	for (int sq = 0; sq < 64; sq++) {
		uint64_t east = shift_east(knight);
		uint64_t west = shift_west(knight);
		uint64_t attacks = shift_nort(shift_nort(east | west));
		attacks |= shift_sout(shift_sout(east | west));
		east = shift_east(east);
		west = shift_west(west);
		attacks |= shift_nort(east | west);
		attacks |= shift_sout(east | west);
		knight_attacks[sq] = attacks;
		knight <<= 1;
	}
}

void print_bitboard(uint64_t& bitboard) {
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			if (file == 0) {
				std::cout << rank + 1 << "   ";
			}
			if (has_sq(bitboard, get_sq_bitboard(get_sq(rank, file)))) {
				std::cout << "X ";
			} 
			else {
				std::cout << "- ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "    a b c d e f g h" << std::endl << std::endl;
}


