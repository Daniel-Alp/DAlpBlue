#include "parser.h"
#include "uci.h"
#include <iostream>
#include <string>
#include <vector>

void uci_loop() {
	std::string command;
	std::vector<std::string> command_sections;
	while (std::getline(std::cin, command)) {
		if (command == "quit") {
			break;
		}
		command_sections = split_string(command, ' ');

	}
}

void uci_go_command(std::string& command) {

}

void uci_position_command(std::string& command) {

}