#include "types.h"

Piece symbol_to_pce(char symbol) {
	switch (symbol) {
	case 'P':
		return Piece::WHITE_PAWN;
	case 'N':
		return Piece::WHITE_KNIGHT;
	case 'B':
		return Piece::WHITE_BISHOP;
	case 'R':
		return Piece::WHITE_ROOK;
	case 'Q':
		return Piece::WHITE_QUEEN;
	case 'K':
		return Piece::WHITE_KING;
	case 'p':
		return Piece::BLACK_PAWN;
	case 'n':
		return Piece::BLACK_KNIGHT;
	case 'b':
		return Piece::BLACK_BISHOP;
	case 'r':
		return Piece::BLACK_ROOK;
	case 'q':
		return Piece::BLACK_QUEEN;
	case 'k':
		return Piece::BLACK_KING;
	}
}

char pce_to_symbol(Piece piece) {
	switch (piece) {
	case Piece::WHITE_PAWN:
		return 'P';
	case Piece::WHITE_KNIGHT:
		return 'N';
	case Piece::WHITE_BISHOP:
		return 'B';
	case Piece::WHITE_ROOK:
		return 'R';
	case Piece::WHITE_QUEEN:
		return 'Q';
	case Piece::WHITE_KING:
		return 'K';
	case Piece::BLACK_PAWN:
		return 'p';
	case Piece::BLACK_KNIGHT:
		return 'n';
	case Piece::BLACK_BISHOP:
		return 'b';
	case Piece::BLACK_ROOK:
		return 'r';
	case Piece::BLACK_QUEEN:
		return 'q';
	case Piece::BLACK_KING:
		return 'k';
	}
}