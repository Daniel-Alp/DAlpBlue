#pragma once

#include"board.h"
#include<cstdint>
#include<array>

constexpr std::array<int32_t,7> midgame_pce_vals = { 0, 82, 337, 365, 477, 1025, 0 };

int32_t evaluate(Position& pos);