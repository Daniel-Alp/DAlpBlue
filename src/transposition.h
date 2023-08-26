#pragma once
#include <array>
#include <cstdint>

enum class HashFlag
{
	EXACT, 
	ALPHA,
	BETA
};

struct HashEntry {
	uint64_t zobrist_key;
	int depth;
	int32_t score;
	HashFlag hash_flag;
	uint32_t best_move;
};

constexpr uint64_t num_hash_entries = 2097152;

extern std::array<HashEntry, num_hash_entries> hash_table;

void clr_hash_table();
void record_hash_entry(uint64_t zobrist_key, int depth, int32_t score, HashFlag hash_flag, uint32_t best_move);