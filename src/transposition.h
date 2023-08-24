#pragma once
#include <array>
#include <cstdint>

struct HashEntry {
	uint64_t zobrist_key;
	uint64_t perft_nodes;
};

constexpr uint64_t num_hash_entries = 8388608;

extern std::array<HashEntry, num_hash_entries> hash_table;

void clr_hash_table();
void record_hash_entry(uint64_t zobrist_key, uint64_t perft_nodes);