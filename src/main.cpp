#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include "uci.h"
#include "zobrist.h"

#include "move.h"
#include "makemove.h"
#include "transposition.h"

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	precompute_zobrist();

	uint64_t nodes = run_perft_with_hash_table(pos, 5, 0);

	//run_perft_suite();
	//uci_loop();
	return 0;
}