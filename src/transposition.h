#pragma once
#include "move.h"
#include "constants.h"
#include <array>
#include <cstdint>

enum class HashFlag {
	EXACT, 
	ALPHA,
	BETA
};

struct HashEntry {
	uint64_t zobrist_key;
	int depth;
	int32_t score;
	HashFlag hash_flag;
	Move best_move;
};

constexpr uint64_t num_hash_entries = 2097152;

extern std::array<HashEntry, num_hash_entries> hash_table;

void clr_hash_table();

inline void record_hash_entry(uint64_t zobrist_key, int depth, int32_t score, HashFlag hash_flag, Move best_move) {
	int entry_index = zobrist_key % num_hash_entries;
	hash_table[entry_index].zobrist_key = zobrist_key;
	hash_table[entry_index].depth = depth;
	hash_table[entry_index].score = score;
	hash_table[entry_index].hash_flag = hash_flag;
	hash_table[entry_index].best_move = best_move;
}

inline int32_t score_to_hash_table(int32_t score, int ply) {
	if (score >= mate_score - max_search_ply) {
		return score + ply;
	}
	if (score <= -mate_score + max_search_ply) {
		return score - ply;
	}
	return score;
}

inline int32_t hash_table_to_score(int32_t score, int ply) {
	if (score >= mate_score - max_search_ply) {
		return score - ply;
	}
	if (score <= -mate_score + max_search_ply) {
		return score + ply;
	}
	return score;
}