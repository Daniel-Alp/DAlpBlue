#include "timemanagement.h"
#include <cstdint>

int64_t get_time_allotted(int64_t player_time, int64_t opp_time, int64_t player_inc, int64_t opp_inc, int moves_to_go) {
	//Basic time management for now
	int64_t time_allotted = (player_time + 1) >> 5;
	return time_allotted;
}

