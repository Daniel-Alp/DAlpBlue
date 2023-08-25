#pragma once

#include <cstdint>
#include <chrono>

int64_t get_time_allotted(int64_t player_time, int64_t opp_time, int64_t player_inc, int64_t opp_inc, int moves_to_go);

inline int64_t get_current_time() {
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::steady_clock().now().time_since_epoch()).count();
}