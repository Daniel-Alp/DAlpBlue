#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "types.h"
#include <iostream>

int main(int argc) {
	precompute_rays();

	uint32_t move = build_move(0, 63, Piece::WHITE_KNIGHT, Piece::BLACK_QUEEN, flag_en_passant);
	std::cout << get_move_str(move) << std::endl;

	/*Position pos{};
	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	load_from_fen(pos, start_fen);
	print_board(pos);*/

	return 0;
}