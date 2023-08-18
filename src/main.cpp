#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "types.h"
#include <iostream>

int main(int argc) {
	precompute_rays();

	uint32_t move = build_move(20, 40, Piece::WHITE_KNIGHT, Piece::BLACK_QUEEN, flag_en_passant);
	std::cout << move_from_sq(move) << std::endl;
	std::cout << move_to_sq(move) << std::endl;
	std::cout << static_cast<uint32_t>(move_capture_pce(move)) << std::endl;
	std::cout << static_cast<uint32_t>(move_promo_pce(move)) << std::endl;
	std::cout << (move & flag_en_passant) << std::endl;
	std::cout << (move & flag_pawn_start) << std::endl;
	std::cout << (move & flag_castle) << std::endl;


	/*Position pos{};
	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	load_from_fen(pos, start_fen);
	print_board(pos);*/

	return 0;
}