#include "bitboard.h"
#include "board.h"
#include "movegen.h"
#include "perft.h"
#include <chrono>
#include <iostream>

int main(int argc) {
	precompute_rays();
	precompute_non_slider_attacks();

	const std::string start_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	const std::string kiwipete_fen = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";

	Position pos{};
	load_from_fen(pos, start_fen);

	const auto time_1 = std::chrono::high_resolution_clock::now();
	uint64_t nodes = run_perft(pos, 6, 0);
	const auto time_2 = std::chrono::high_resolution_clock::now();
	auto elapsed_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_2 - time_1);

	std::cout << "nodes: " << nodes << " time: " << elapsed_time_ms.count() << std::endl;
	return 0;
}