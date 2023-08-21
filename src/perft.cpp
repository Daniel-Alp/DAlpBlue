#include "movegen.h"
#include "makemove.h"
#include "parser.h"
#include "perft.h"
#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>


uint64_t run_perft(Position& pos, int depth, int ply) {
	if (depth == 0) {
		return 1;
	}

	uint64_t nodes = 0;
	std::array<uint32_t, 218> moves;
	int num_moves;
	gen_pseudo_moves(pos, moves, num_moves, false);

	for (int i = 0; i < num_moves; i++) {
		uint32_t move = moves[i];
		if (make_move(pos, move)) {
			nodes += run_perft(pos, depth - 1, ply + 1);
			undo_move(pos, move);
		}
	}
	return nodes;
}

void run_perft_suite() {
	std::ifstream perft_test_suite;
	perft_test_suite.open("perftsuite.txt");
	std::string test;
	while (std::getline(perft_test_suite, test)) {
		std::vector<std::string> test_sections;
		test_sections = split_string(test, ';');

		std::string fen = test_sections[0];
		Position pos{};
		load_from_fen(pos, fen);

		std::cout << fen << std::endl;

		int depth = test_sections.size() - 1;

		uint64_t expect = std::stol(test_sections[depth].substr(3));

		auto time_1 = std::chrono::high_resolution_clock::now();
		uint64_t result = run_perft(pos, depth, 0);
		auto time_2 = std::chrono::high_resolution_clock::now();
		auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_2 - time_1);

		std::cout << "D" << std::setw(5) << std::left << depth
			<< "EXP: " << std::setw(15) << std::left << expect
			<< "RES: " << std::setw(15) << std::left << result
			<< "MS:  " << std::setw(15) << std::left << elapsed_time_ms.count()
			<< (expect == result ? "PASS" : "FAIL") << std::endl << std::endl;
	}
}