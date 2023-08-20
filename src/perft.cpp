#include "perft.h"
#include "movegen.h"
#include "makemove.h"
#include <array>
#include <cstdint>

uint64_t run_perft(Position& pos, int depth, int ply) {
	if (depth == 0) {
		return 1;
	}

	uint64_t nodes = 0;
	std::array<uint32_t, 218> moves;
	int num_moves;
	gen_pseudo_moves(pos, moves, num_moves, false);

	for (int i = 0; i < num_moves; i++) {
		uint32_t move = moves[i];
		if (make_move(pos, move)) {
			nodes += run_perft(pos, depth - 1, ply + 1);
			undo_move(pos, move);
		}
	}
	return nodes;
}