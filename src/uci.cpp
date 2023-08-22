#include "board.h"
#include "search.h"
#include "parser.h"
#include "uci.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void uci_loop() {
	Position pos;
	std::thread search_thread;

	std::string command;
	std::string command_type;
	std::vector<std::string> command_sections;
	while (std::getline(std::cin, command)) {
		if (command == "quit") {
			break;
		}
		command_sections = split_string(command, ' ');
		command_type = command_sections[0];
		if (command_type == "uci") {
			std::cout << "id name DAlpBlue" << std::endl;
			std::cout << "id author Daniel Alp" << std::endl;
			std::cout << "uciok" << std::endl;
		}
		else if (command_type == "isready") {
			std::cout << "readyok" << std::endl;
		} 
		else if (command_type == "ucinewgame") {
			pos = {};
		}
		else if (command_type == "position") {
			uci_position_command(command_sections);
		}
		else if (command_type == "go") {
			uci_go_command(command_sections, search_thread, pos);
		}
		else if (command_type == "stop") {
			searching = false;
			search_thread.join();
		}
		else {
			std::cout << "Unknown command: " << command << std::endl;
		}
	}
}

void uci_go_command(std::vector<std::string>& command_sections, std::thread& search_thread, Position& pos) {
	searching = true;
	search_thread = std::thread(get_best_move, std::ref(pos));
}

void uci_position_command(std::vector<std::string>& command_sections) {

}