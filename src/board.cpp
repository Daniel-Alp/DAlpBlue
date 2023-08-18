#include "board.h"
#include "types.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

void load_from_fen(Position& pos, std::string fen_string) {
	std::vector<std::string> fen_sections;
	std::istringstream iss(fen_string);
	std::string section;
	while (!iss.eof()) {
		std::getline(iss, section, ' ');
		fen_sections.push_back(section);
	}

	int rank = 7;
	int file = 0;
	for (char symbol : fen_sections[0]) {
		if (symbol == '/') {
			rank--;
			file = 0;
		}
		else if (isdigit(symbol)) {
			for (int i = 0; i < symbol - '0'; i++) {
				int sq = rank * 8 + file + i;
				pos.pces[sq] = Piece::NONE;
			}
			file += symbol - '0';
		}
		else {
			int sq = rank * 8 + file;
			pos.pces[sq] = symbol_to_pce(symbol);
			file++;
		}
	}
}

void print_board(Position& pos) {
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			if (file == 0) {
				std::cout << rank + 1 << "   ";
			}
			if (pos.pces[rank * 8 + file] == Piece::NONE) {
				std::cout << ". ";
			}
			else {
				std::cout << pce_to_symbol(pos.pces[rank * 8 + file]) << " ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << "    a b c d e f g h" << std::endl;
}
