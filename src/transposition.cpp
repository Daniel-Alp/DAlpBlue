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