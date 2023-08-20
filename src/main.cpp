#include "bitboard.h"
#include "board.h"
#include "perft.h"
#include <string>

int main(int argc, char* argv[]) {
	precompute_rays();
	precompute_non_slider_attacks();
	run_perft_suite();

	//const auto time_1 = std::chrono::high_resolution_clock::now();
	//uint64_t nodes = run_perft(pos, 6, 0);
	//const auto time_2 = std::chrono::high_resolution_clock::now();
	//auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_2 - time_1);

	//std::cout << "nodes: " << nodes << " time: " << elapsed_time_ms.count() << std::endl;
	return 0;
}