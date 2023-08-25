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

	std::array<uint32_t, max_moves> moves;
	int num_moves = 218;
	std::array<uint32_t, max_moves> scores{};
	scores[0] = 54;
	scores[1] = 88;
	scores[2] = 8;
	scores[3] = 17;
	scores[4] = 53;
	scores[5] = 13;
	scores[6] = 60;
	scores[7] = 60;
	scores[8] = 30;
	scores[9] = 30;
	scores[217] = 89;

	for (int i = 0; i < num_moves; i++) {
		order_moves(moves, num_moves, scores, i);
	}

	std::cout << scores[0];


	//uci_loop();
	return 0;
}