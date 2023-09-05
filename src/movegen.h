#pragma once

#include "bitboard.h"
#include "move.h"
#include "board.h"
#include "attacks.h"
#include <array>

constexpr inline int max_moves = 218;

void gen_pseudo_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, bool not_quiet);
void serialize_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, uint64_t attacks, int from_sq);
void serialize_pawn_promo(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, uint64_t to_sqs, const int dir);
void serialize_pawn_non_promo(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, uint64_t to_sqs, const int dir, const MoveFlag flag);
void gen_pawn_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, const Color col);
void gen_king_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, uint64_t king);
void gen_knight_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, uint64_t knights);
void gen_bishop_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, uint64_t bishops);
void gen_rook_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, uint64_t bishops);
void gen_queen_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves, const uint64_t targets, uint64_t bishops);
void gen_castling_moves(const Position& pos, std::array<Move, max_moves>& moves, int& num_moves);