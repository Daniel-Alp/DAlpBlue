#include "bitboard.h"
#include "board.h"
#include "evaluation.h"
#include "perft.h"
#include "types.h"
#include "uci.h"
#include "zobrist.h"

#include "movegen.h"
#include "makemove.h"

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();
	precompute_zobrist();

	precompute_pce_psqt(Piece::WHITE_PAWN, pawn_midgame_psqt, pawn_endgame_psqt);
	precompute_pce_psqt(Piece::BLACK_PAWN, pawn_midgame_psqt, pawn_endgame_psqt);

	precompute_pce_psqt(Piece::WHITE_KNIGHT, knight_midgame_psqt, knight_endgame_psqt);
	precompute_pce_psqt(Piece::BLACK_KNIGHT, knight_midgame_psqt, knight_endgame_psqt);

	precompute_pce_psqt(Piece::WHITE_BISHOP, bishop_midgame_psqt, bishop_endgame_psqt);
	precompute_pce_psqt(Piece::BLACK_BISHOP, bishop_midgame_psqt, bishop_endgame_psqt);

	precompute_pce_psqt(Piece::WHITE_ROOK, rook_midgame_psqt, rook_endgame_psqt);
	precompute_pce_psqt(Piece::BLACK_ROOK, rook_midgame_psqt, rook_endgame_psqt);

	precompute_pce_psqt(Piece::WHITE_QUEEN, queen_midgame_psqt, queen_endgame_psqt);
	precompute_pce_psqt(Piece::BLACK_QUEEN, queen_midgame_psqt, queen_endgame_psqt);

	precompute_pce_psqt(Piece::WHITE_KING, king_midgame_psqt, king_endgame_psqt);
	precompute_pce_psqt(Piece::BLACK_KING, king_midgame_psqt, king_endgame_psqt);

	//Position pos{}; //DA!!! if king is not on the board, gets LSB of king bitboard as 0
	//std::string fen = "1R6/8/4K3/8/4b3/pk6/4r3/8 b - - 0 1";

	//load_from_fen(pos, fen);

	//print_board(pos);

	//uint64_t nodes = run_perft(pos, 9, 0); // seems to pass this
	// print_board(pos) // run witth this line of code after
	//uint64_t nodes = run_perft(pos, 10, 0); // seems to pass this


	//std::cout << nodes << std::endl;

	//run_perft_suite();

	uci_loop();
	return 0;
}