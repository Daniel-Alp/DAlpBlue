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

class HashTable {
public:
	inline void clear() {
		for (int i = 0; i < num_hash_entries; i++) {
			hash_table[i].zobrist_key = 0;
		}
	}

	inline void record(uint64_t zobrist_key, int depth, int32_t score, HashFlag hash_flag, Move best_move) {
		const int i = zobrist_key % num_hash_entries;
		hash_table[i].zobrist_key = zobrist_key;
		hash_table[i].depth = depth;
		hash_table[i].score = score;
		hash_table[i].hash_flag = hash_flag;
		hash_table[i].best_move = best_move;
	}

	inline HashEntry get(uint64_t zobrist_key) const {
		return hash_table[zobrist_key % num_hash_entries];
	}

	inline int32_t score_to_hash_table(int32_t score, int ply) const {
		if (score >= mate_score - max_search_ply) {
			return score + ply;
		}
		if (score <= -mate_score + max_search_ply) {
			return score - ply;
		}
		return score;
	} 

	inline int32_t hash_table_to_score(int32_t score, int ply) const {
		if (score >= mate_score - max_search_ply) {
			return score - ply;
		}
		if (score <= -mate_score + max_search_ply) {
			return score + ply;
		}
		return score;
	}

private:
	std::array<HashEntry, num_hash_entries> hash_table;
};

inline HashTable hash_table;