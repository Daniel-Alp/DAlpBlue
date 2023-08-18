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
	NONE,
	WHITE_PAWN,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING
};

constexpr inline Color get_col(Piece piece) {
	return static_cast<Color>(static_cast<uint32_t>(piece) >> 3);
}
constexpr inline PieceType get_pce_type(Piece piece, Color col) {
	return static_cast<PieceType>(static_cast<uint32_t>(piece) - static_cast<uint32_t>(col) * 6);
}
constexpr inline Piece build_pce(PieceType pce_type, Color col) {
	return static_cast<Piece>(static_cast<uint32_t>(pce_type) + static_cast<uint32_t>(col) << 3);
}
Piece symbol_to_pce(char symbol);
char pce_to_symbol(Piece piece);