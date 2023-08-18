#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "types.h"
#include <iostream>

int main(int argc) {
	precompute_rays();

	Position pos{};
	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	load_from_fen(pos, start_fen);
	print_board(pos);

	return 0;
}