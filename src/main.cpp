#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "movegen.h"
#include "types.h"
#include <array>
#include <iostream>

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();

	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	Position pos{};
	load_from_fen(pos, "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1");
	print_board(pos);

	std::array<uint32_t, max_moves> moves;
	int num_moves;
	gen_pseudo_moves(pos, moves, num_moves, false);
	print_moves(moves, num_moves);

	return 0;
}