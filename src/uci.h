#pragma once

#include <string>

void uci_loop();
void uci_go_command(std::string& msg);
void uci_position_command(std::string& msg);