#pragma once

#include "board.h"
#include <string>
#include <thread>
#include <vector>

void uci_loop();
void uci_go_command(std::vector<std::string>& command_sections, std::thread& search_thread, Position& pos);
bool uci_position_command(std::vector<std::string>& command_sections, Position& pos);