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

	int num_moves = 10;
	std::array<uint32_t, max_moves> moves{};
	moves[0] = 0;
	moves[1] = 1;
	moves[2] = 2;
	moves[3] = 3;
	moves[4] = 4;
	moves[5] = 5;
	moves[6] = 6;
	moves[7] = 7;
	moves[8] = 8;
	moves[9] = 9;

	std::array<int32_t, max_moves> scores{};
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
	//(1, 88) (6, 60) (7, 60) (0, 54) (4, 53) (8, 30) (9, 30) (3, 17) (5, 13) (2, 8)

	//scores[217] = 89;

	for (int i = 0; i < num_moves; i++) {
		order_moves(moves, num_moves, scores, i);
		//std::cout << best_move_index << std::endl;
		//std::cout << scores[i] << std::endl;
		std::cout << moves[i] << std::endl;
		std::cout << std::endl;
	}

	//it sorts correctly, but returns the wrong move index
	for (int i = 0; i < num_moves; i ++) {
		std::cout << scores[i] << " ";
	}

	uci_loop();
	return 0;
}