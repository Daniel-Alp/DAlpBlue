#include "move.h"
#include "movegen.h"
#include "makemove.h"
#include "parser.h"
#include "perft.h"
#include "transposition.h"
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
	MoveList move_list = gen_pseudo_moves(pos, false);

	for (int i = 0; i < move_list.size(); i++) {
		Move move = move_list.get(i);
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
		const std::vector<std::string> test_sections = split_string(test, ';');

		const std::string fen = test_sections[0];
		Position pos = load_from_fen(fen);
		hash_table.clear();

		std::cout << fen << std::endl;
		const int depth = test_sections.size() - 1;
		const uint64_t expect = std::stol(test_sections[depth].substr(3));

		const auto time_1 = std::chrono::high_resolution_clock::now();
		const uint64_t result = run_perft(pos, depth, 0);
		const auto time_2 = std::chrono::high_resolution_clock::now();
		const auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_2 - time_1);

		std::cout << "D"     << std::setw(5)  << std::left << depth
				  << "EXP: " << std::setw(15) << std::left << expect
			      << "RES: " << std::setw(15) << std::left << result
			      << "MS:  " << std::setw(15) << std::left << elapsed_time_ms.count()
			      << (expect == result ? "PASS" : "FAIL") << std::endl << std::endl;
	}
}