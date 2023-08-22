#include "search.h"
#include "move.h"

#include <iostream>

bool searching = true;

uint32_t get_best_move(Position& pos) {
	uint32_t best_move = null_move;
	
	uint64_t i = 0;
	while (searching) {
		i++;
	}
	std::cout << "Counted to: " << i << std::endl;

	return best_move;
}