#include "board.h"
#include "types.h"

static const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";

int main(int argc) {
	Position pos;
	load_from_fen(pos, START_FEN);
	print_board(pos);
	return 0;
}