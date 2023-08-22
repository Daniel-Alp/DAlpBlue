#pragma once

#include <string>
#include <vector>

void uci_loop();
void uci_go_command(std::vector<std::string>& command_sections);
void uci_position_command(std::vector<std::string>& command_sections);