#include "search.h"
#include "move.h"
#include "movegen.h"
#include "makemove.h"

#include "cstdlib"

#include <iostream>

bool searching;

void get_best_move(Position& pos) {
	searching = true;
	pos.ply = 0;
	uint32_t best_move = null_move;

	//Returns random legal move in position
	std::array<uint32_t, max_moves> pseudo_moves;
	int num_pseudo_moves;
	gen_pseudo_moves(pos, pseudo_moves, num_pseudo_moves, false);

	std::array<uint32_t, max_moves> legal_moves;
	int num_legal_moves = 0;

	for (int i = 0; i < num_pseudo_moves; i++) {
		if (make_move(pos, pseudo_moves[i])) {
			undo_move(pos, pseudo_moves[i]);
			legal_moves[num_legal_moves++] = pseudo_moves[i];
		}
	}

	srand(time(NULL));

	searching = false;
	std::cout << "bestmove " << get_move_str(legal_moves[rand() % num_legal_moves]) << std::endl;
}