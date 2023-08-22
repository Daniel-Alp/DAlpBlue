#include "search.h"
#include "move.h"

#include <iostream>

bool searching;

void get_best_move(Position& pos) {
	searching = true;
	uint32_t best_move = null_move;

	uint64_t i = 0;
	while (searching) {
		i++;
		if (i > 10000000000) {
			break;
		}
	}
	std::cout << "Counted to: " << i << std::endl;
	searching = false;
	std::cout << "bestmove " << get_move_str(best_move) << std::endl;
}