#include "transposition.h"
#include <array>
#include <cstdint>

#include <iostream>

std::array<HashEntry, num_hash_entries> hash_table;

void clr_hash_table() {
	for (int entry_index = 0; entry_index < num_hash_entries; entry_index++) {
		hash_table[entry_index].zobrist_key = 0;
	}
}

void record_hash_entry(uint64_t zobrist_key, int depth, int32_t score, HashFlag hash_flag, uint32_t best_move) {
	int entry_index = zobrist_key & (num_hash_entries - 1);
	hash_table[entry_index].zobrist_key = zobrist_key;
	hash_table[entry_index].depth = depth;
	hash_table[entry_index].score = score;
	hash_table[entry_index].hash_flag = hash_flag;
	hash_table[entry_index].best_move = best_move;
}