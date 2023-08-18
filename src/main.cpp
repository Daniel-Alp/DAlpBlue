#include "bitboard.h"
#include "board.h"
#include "types.h"
#include <iostream>

//TODO add debug statements
int main(int argc) {
	precompute_rays();

	Position pos{};
	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	load_from_fen(pos, start_fen);
	print_board(pos);
	print_bitboard(pos.all_bitboard);
	print_bitboard(pos.col_bitboards[0]);
	print_bitboard(pos.col_bitboards[1]);

	return 0;
}