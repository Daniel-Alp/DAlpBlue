#pragma once

#include "board.h"
#include <cstdint>

extern bool searching;

uint32_t get_best_move(Position& pos);