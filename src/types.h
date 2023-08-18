#pragma once

#include <cstdint>

enum class Color : uint32_t {
	WHITE,
	BLACK
};

enum class PieceType : uint32_t {
	NONE,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum class Piece : uint32_t {
	NONE = 0,
	WHITE_PAWN = 1,
	WHITE_KNIGHT = 2,
	WHITE_BISHOP = 3,
	WHITE_ROOK = 4,
	WHITE_QUEEN = 5,
	WHITE_KING = 6,
	BLACK_PAWN = 9,
	BLACK_KNIGHT = 10,
	BLACK_BISHOP = 11,
	BLACK_ROOK = 12,
	BLACK_QUEEN = 13,
	BLACK_KING = 14
};

enum class CastlingRights : uint32_t {
	WHITE_SHORT = 1,
	WHITE_LONG	= 2,
	BLACK_SHORT = 4,
	BLACK_LONG	= 8
};

Piece symbol_to_pce(char symbol);
char pce_to_symbol(Piece piece);
constexpr Color get_col(Piece piece) {
	return static_cast<Color>(static_cast<uint32_t>(piece) >> 3);
}
constexpr PieceType get_pce_type(Piece piece, Color col) {
	return static_cast<PieceType>(static_cast<uint32_t>(piece) & 7);
}
constexpr Piece build_pce(PieceType pce_type, Color col) {
	return static_cast<Piece>(static_cast<uint32_t>(pce_type) + (static_cast<uint32_t>(col) << 3));
}
