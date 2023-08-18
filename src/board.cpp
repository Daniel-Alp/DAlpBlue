#include "bitboard.h"
#include "board.h"
#include "types.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void load_from_fen(Position& pos, const std::string& fen_string) {
	std::vector<std::string> fen_sections;
	std::istringstream iss(fen_string);
	std::string section;
	while (!iss.eof()) {
		std::getline(iss, section, ' ');
		fen_sections.push_back(section);
	}

	const std::string piece_placement_section = fen_sections[0];
	const std::string side_to_move_section    = fen_sections[1];
	const std::string castling_rights_section = fen_sections[2];
	const std::string en_passant_sq_section   = fen_sections[3];
	const std::string fifty_move_rule_section = fen_sections[4];

	int rank = 7;
	int file = 0;
	for (const char symbol : piece_placement_section) {
		if (symbol == '/') {
			rank--;
			file = 0;
		}
		else if (isdigit(symbol)) {
			for (uint32_t i = 0; i < symbol - '0'; i++) {
				pos.pces[get_sq(rank, file + i)] = Piece::NONE;
			}
			file += symbol - '0';
		}
		else {
			const int sq = get_sq(rank, file);
			const uint64_t sqBB = get_sq_bitboard(sq);
			const Piece pce = symbol_to_pce(symbol);

			pos.pce_bitboards[static_cast<uint32_t>(pce)] = set_sq(pos.pce_bitboards[static_cast<uint32_t>(pce)], sqBB);
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
		const uint32_t rank = en_passant_sq_section[1] - '1';
		const uint32_t file = en_passant_sq_section[0] - 'a';
		pos.en_passant_sq = get_sq(rank, file);
	}

	pos.fifty_move_rule = std::stoi(fifty_move_rule_section);
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
