#include "transposition.h"
#include <array>
#include <cstdint>

std::array<HashEntry, num_hash_entries> hash_table;

void clr_hash_table() {
	for (int entry_index = 0; entry_index < num_hash_entries; entry_index++) {
		hash_table[entry_index].zobrist_key = 0;
	}
}

void record_hash_entry(uint64_t zobrist_key, uint64_t perft_nodes) {
	int entry_index = zobrist_key % num_hash_entries;
	hash_table[entry_index].zobrist_key = zobrist_key;
	hash_table[entry_index].perft_nodes = perft_nodes;
}