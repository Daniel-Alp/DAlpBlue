#pragma once

#include "board.h"
#include "movegen.h"
#include "transposition.h"

constexpr int32_t mate_score = 30000;

struct SearchData {
	bool searching;
	uint64_t start_time;
	uint64_t time_allotted;
	uint64_t nodes;
};

void get_best_move(Position& pos, SearchData& search_data);

void test_search(Position& pos, SearchData& search_data, int depth);