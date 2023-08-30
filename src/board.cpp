#include "bitboard.h"
#include "board.h"
#include "evaluation.h"
#include "parser.h"
#include "types.h"
#include "zobrist.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Position load_from_fen(std::string& fen_string) {
	Position pos{};

	std::vector<std::string> fen_sections;
	fen_sections = split_string(fen_string, ' ');

	std::string piece_placement_section = fen_sections[0];
	std::string side_to_move_section = fen_sections[1];
	std::string castling_rights_section = fen_sections[2];
	std::string en_passant_sq_section = fen_sections[3];
	std::string fifty_move_rule_section = fen_sections[4];

	for (int sq = 0; sq < 64; sq++) {
		pos.pces[sq] = Piece::NONE;
	}
	int rank = 7;
	int file = 0;
	for (char symbol : piece_placement_section) {
		if (symbol == '/') {
			rank--;
			file = 0;
		}
		else if (isdigit(symbol)) {
			file += symbol - '0';
		}
		else {
			int sq = get_sq(rank, file);
			uint64_t sq_bb = get_sq_bitboard(sq);
			Piece pce = symbol_to_pce(symbol);
			Color col = get_col(pce);

			pos.all_bitboard = set_sq(pos.all_bitboard, sq_bb);
			pos.col_bitboards[static_cast<int>(col)] = set_sq(pos.col_bitboards[static_cast<int>(col)], sq_bb);
			pos.pce_bitboards[static_cast<int>(pce)] = set_sq(pos.pce_bitboards[static_cast<int>(pce)], sq_bb);
			pos.pces[sq] = pce;

			pos.material_midgame_val += material_midgame_vals[static_cast<int>(pce)];
			pos.material_endgame_val += material_endgame_vals[static_cast<int>(pce)];
			pos.phase_val += phase_vals[static_cast<int>(pce)];
			pos.phase_val = std::min(pos.phase_val, 24);
			pos.psqt_midgame_val += pce_psqts_midgame[static_cast<int>(pce)][sq];
			pos.psqt_endgame_val += pce_psqts_endgame[static_cast<int>(pce)][sq];

			file++;
		}
	}

	if (side_to_move_section == "w") {
		pos.side_to_move = Color::WHITE;
	}
	else {
		pos.side_to_move = Color::BLACK;
	}

	if (castling_rights_section.find('K') != std::string::npos) {
		pos.castling_rights |= static_cast<int>(CastlingRights::WHITE_SHORT);
	}
	if (castling_rights_section.find('Q') != std::string::npos) {
		pos.castling_rights |= static_cast<int>(CastlingRights::WHITE_LONG);
	}
	if (castling_rights_section.find('k') != std::string::npos) {
		pos.castling_rights |= static_cast<int>(CastlingRights::BLACK_SHORT);
	}
	if (castling_rights_section.find('q') != std::string::npos) {
		pos.castling_rights |= static_cast<int>(CastlingRights::BLACK_LONG);
	}

	if (en_passant_sq_section == "-") {
		pos.en_passant_sq = static_cast<int>(Square::NO_SQ);
	}
	else {
		uint32_t rank = en_passant_sq_section[1] - '1';
		uint32_t file = en_passant_sq_section[0] - 'a';
		pos.en_passant_sq = get_sq(rank, file);
	}

	pos.fifty_move_rule = std::stoi(fifty_move_rule_section);

	pos.zobrist_key = get_zobrist_key(pos);

	return pos;
}

void print_board(Position& pos) {
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			if (file == 0) {
				std::cout << rank + 1 << "   ";
			}
			if (pos.pces[get_sq(rank, file)] == Piece::NONE) {
				std::cout << ". ";
			}
			else {
				std::cout << pce_to_symbol(pos.pces[get_sq(rank, file)]) << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "    a b c d e f g h" << std::endl << std::endl;
}

bool valid_pos(Position& pos) { 
	for (int sq = 0; sq < 64; sq++) {
		uint64_t bb_sq = get_sq_bitboard(sq);
		Piece pce_at_sq = pos.pces[sq];

		if (pce_at_sq == Piece::NONE) {
			//Check that none of the bitboards contains the square
			if (has_sq(pos.all_bitboard, sq)) { 
				return false;
			}

			for (int col = static_cast<int>(Color::WHITE); col <= static_cast<int>(Color::BLACK); col++) {
				for (int pce_type = static_cast<int>(PieceType::PAWN); pce_type <= static_cast<int>(PieceType::KING); pce_type++) {
					int pce = build_pce(pce_type, col);
					if (has_sq(pos.pce_bitboards[pce], sq)) {
						return false;
					}
				}
				if (has_sq(pos.col_bitboards[col], sq)) {
					return false;
				}
			}
		}
		else {
			//Check that all bitboard contains square
			if (!has_sq(pos.all_bitboard, sq)) {
				return false;
			}
			//Check that bitboard of pce col contains square
			if (!has_sq(pos.col_bitboards[static_cast<int>(get_col(pce_at_sq))], sq)) {
				return false;
			}
			//Check that bitboard of not pce col does not contain square
			if (has_sq(pos.col_bitboards[static_cast<int>(flip_col(get_col(pce_at_sq)))], sq)) {
				return false;
			}
			//Check that pce bitboard contains square
			if (!has_sq(pos.pce_bitboards[static_cast<int>(pce_at_sq)], sq)) {
				return false;
			}
			//Check all other pce bitboard does not contain square
			for (int col = static_cast<int>(Color::WHITE); col <= static_cast<int>(Color::BLACK); col++) {
				for (int pce_type = static_cast<int>(PieceType::PAWN); pce_type <= static_cast<int>(PieceType::KING); pce_type++) {
					int pce = build_pce(pce_type, col);
					if (pce != static_cast<int>(pce_at_sq) && has_sq(pos.pce_bitboards[pce], sq)) {
						return false;
					}
				}
			}
		}
	}

	return true;
}