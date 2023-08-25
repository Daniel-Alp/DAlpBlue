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
	int num_moves = 10;
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


	//orig
	//(0, 54) (1, 88) (2, 8) (3, 17) (4, 53) (5, 13) (6, 60) (7, 60) (8, 30) (9, 30)
	// sorted
	//(1, 88) (6, 60) (7, 60) (0, 54) (4, 53) (8, 30) (9, 30) (3, 17) (6, 13) (2, 8)

	//scores[217] = 89;

	for (int i = 0; i < num_moves; i++) {
		int32_t best_move_index = order_moves(num_moves, scores, i);
		std::cout << best_move_index << std::endl;
		std::cout << scores[best_move_index] << std::endl;
		std::cout << std::endl;
	}

	uci_loop();
	return 0;
}