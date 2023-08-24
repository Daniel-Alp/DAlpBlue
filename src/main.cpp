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

	Position pos{};
	load_from_fen(pos, start_fen);//std::string("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

	//PASSES UP TO DEPTH 5, FAILS AT DEPTH 6
	//UPDATE: WEIRD BUG, IT ALWAYS MANAGES TO LOOK 4 MOVES AHEAD, BUT FAILS AT 5

	//Knight moves seem to be completely fucked
	clr_hash_table();
	uint64_t nodes = run_perft_with_hash_table(pos, 4, 0);
	std::cout << nodes << std::endl;
	
	//run_perft_suite();
	//uci_loop();
	return 0;
}