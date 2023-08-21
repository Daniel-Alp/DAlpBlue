#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include "uci.h"

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	//run_perft_suite();
	uci_loop();
	return 0;
}