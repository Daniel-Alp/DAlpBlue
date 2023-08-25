#include "bitboard.h"
#include "board.h"
#include "uci.h"
#include "zobrist.h"

#include "movegen.h"
#include "search.h"

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	precompute_zobrist();
	uci_loop();
	return 0;
}