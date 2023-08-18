#include "board.h"
#include "types.h"
#include <iostream>

//TODO add debug statements
int main(int argc) {
	Position pos{};

	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	load_from_fen(pos, start_fen);
	print_board(pos);

	return 0;
}