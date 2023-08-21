#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include <string>

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	run_perft_suite();
	return 0;
}