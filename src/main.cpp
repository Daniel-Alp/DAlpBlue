#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include "uci.h"
#include "zobrist.h"

#include <iostream>

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	precompute_zobrist();



	//run_perft_suite();
	//uci_loop();
	return 0;
}