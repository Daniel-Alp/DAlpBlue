#pragma once

#include<cstdint>
#include<array>

constexpr uint64_t num_tt_entries = 8388608;

extern std::array<TTEntry, num_tt_entries> ttable;

struct TTEntry {
	uint64_t zobrist_key;
};