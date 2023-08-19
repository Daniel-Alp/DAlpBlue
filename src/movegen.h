#pragma once

#include "bitboard.h"
#include "move.h"
#include "board.h"
#include "attacks.h"
#include <array>

constexpr inline int max_moves = 218;

void gen_pseudo_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, bool not_quiet);
void serialize_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t attacks, int from_sq);
void serialize_pawn_promo(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t to_sqs, int dir);
void serialize_pawn_non_promo(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t to_sqs, int dir, MoveFlag flag);
void gen_pawn_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, Color col);
void gen_king_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t king);
void gen_knight_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t knights);
void gen_bishop_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops);
void gen_rook_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops);
void gen_queen_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves, uint64_t targets, uint64_t bishops);
void gen_castling_moves(Position& pos, std::array<uint32_t, max_moves>& moves, int& num_moves);
void print_moves(std::array<uint32_t, max_moves>& moves, int num_moves);
