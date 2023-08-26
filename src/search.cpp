#include "board.h"
#include "makemove.h"
#include "movegen.h"
#include "search.h"
#include <iostream>

void get_best_move(Position& pos, SearchData& search_data) {
	uint32_t best_move_root = null_move;

	test_search(pos, search_data, 4);
}

//Temporary search method used to test PSQTs
void test_search(Position& pos, SearchData& search_data, int depth) {
	search_data.nodes++;
	if (depth == 0) {
		if (search_data.nodes % 1024 == 0) {
			print_board(pos);
			std::cout << "MIDGAME MTRL " << pos.material_midgame_val << std::endl;
			std::cout << "ENDGAME MTRL " << pos.material_endgame_val << std::endl;
			std::cout << "MIDGAME PSQT " << pos.psqt_midgame_val << std::endl;
			std::cout << "ENDGAME PSQT " << pos.psqt_endgame_val << std::endl;
			std::cout << "PHASE    VAL " << pos.phase_val << std::endl;
			std::cout << std::endl;
		}
		return;
	}

	std::array<uint32_t, max_moves> moves;
	int num_moves;
	gen_pseudo_moves(pos, moves, num_moves, false);

	for (int i = 0; i < num_moves; i++) {
		uint32_t move = moves[i];
		if (make_move(pos, move)) {
			test_search(pos, search_data, depth - 1);
			undo_move(pos, move);
		}
	}
}