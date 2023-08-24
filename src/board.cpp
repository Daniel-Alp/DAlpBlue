#include "bitboard.h"
#include "board.h"
#include "parser.h"
#include "types.h"
#include "zobrist.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void load_from_fen(Position& pos, std::string& fen_string) {
	pos.all_bitboard = 0;
	std::fill(pos.col_bitboards.begin(), pos.col_bitboards.end(), 0);
	std::fill(pos.pce_bitboards.begin(), pos.pce_bitboards.end(), 0);
	for (int sq = 0; sq < 64; sq++) {
		pos.pces[sq] = Piece::NONE;
	}

	std::vector<std::string> fen_sections;
	fen_sections = split_string(fen_string, ' ');

	std::string piece_placement_section = fen_sections[0];
	std::string side_to_move_section = fen_sections[1];
	std::string castling_rights_section = fen_sections[2];
	std::string en_passant_sq_section = fen_sections[3];
	std::string fifty_move_rule_section = fen_sections[4];

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
			pos.col_bitboards[static_cast<uint32_t>(col)] = set_sq(pos.col_bitboards[static_cast<uint32_t>(col)], sq_bb);
			pos.pce_bitboards[static_cast<uint32_t>(pce)] = set_sq(pos.pce_bitboards[static_cast<uint32_t>(pce)], sq_bb);
			pos.pces[sq] = pce;

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
		pos.castling_rights |= static_cast<uint32_t>(CastlingRights::WHITE_SHORT);
	}
	if (castling_rights_section.find('Q') != std::string::npos) {
		pos.castling_rights |= static_cast<uint32_t>(CastlingRights::WHITE_LONG);
	}
	if (castling_rights_section.find('k') != std::string::npos) {
		pos.castling_rights |= static_cast<uint32_t>(CastlingRights::BLACK_SHORT);
	}
	if (castling_rights_section.find('q') != std::string::npos) {
		pos.castling_rights |= static_cast<uint32_t>(CastlingRights::BLACK_LONG);
	}

	if (en_passant_sq_section == "-") {
		pos.en_passant_sq = static_cast<uint32_t>(Square::NO_SQ);
	}
	else {
		uint32_t rank = en_passant_sq_section[1] - '1';
		uint32_t file = en_passant_sq_section[0] - 'a';
		pos.en_passant_sq = get_sq(rank, file);
	}

	pos.fifty_move_rule = std::stoi(fifty_move_rule_section);

	pos.zobrist_key = get_zobrist_key(pos);
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