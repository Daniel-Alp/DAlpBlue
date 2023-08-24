#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include "uci.h"
#include "zobrist.h"

#include "move.h"
#include "makemove.h"
#include "transposition.h"

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	precompute_zobrist();

	Position pos{};
	load_from_fen(pos, start_fen);//std::string("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"));

	std::cout << "zobrist: " << pos.zobrist_key << std::endl;

	//uint32_t move1 = build_move(static_cast<int>(Square::A2), static_cast<int>(Square::A4), Piece::NONE, Piece::NONE, MoveFlag::PAWN_START);
	//uint32_t move2 = build_move(static_cast<int>(Square::A7), static_cast<int>(Square::A6), Piece::NONE, Piece::NONE, MoveFlag::PAWN_START);
	//uint32_t move3 = build_move(static_cast<int>(Square::B2), static_cast<int>(Square::B4), Piece::NONE, Piece::NONE, MoveFlag::PAWN_START);
	//make_move(pos, move1);
	//make_move(pos, move2);
	//make_move(pos, move3);


	//uint32_t move1 = build_move(static_cast<int>(Square::B1), static_cast<int>(Square::C3), Piece::NONE, Piece::NONE, MoveFlag::NONE);
	//uint32_t move2 = build_move(static_cast<int>(Square::B8), static_cast<int>(Square::C6), Piece::NONE, Piece::NONE, MoveFlag::NONE);
	//uint32_t move3 = build_move(static_cast<int>(Square::C3), static_cast<int>(Square::B1), Piece::NONE, Piece::NONE, MoveFlag::NONE);
	//uint32_t move4 = build_move(static_cast<int>(Square::C6), static_cast<int>(Square::B8), Piece::NONE, Piece::NONE, MoveFlag::NONE);
	//make_move(pos, move1);
	//make_move(pos, move2);
	//make_move(pos, move3);
	//make_move(pos, move4);

	print_board(pos);

	std::cout << "zobrist: " << pos.zobrist_key << std::endl;

	//clr_hash_table();
	//uint64_t nodes = run_perft_with_hash_table(pos, 5, 0);

	//std::cout << nodes << std::endl;

	//run_perft_suite();
	//uci_loop();
	return 0;
}