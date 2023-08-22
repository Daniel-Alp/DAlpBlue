#include "board.h"
#include "search.h"
#include "parser.h"
#include "uci.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void uci_loop() {

}

/*void uci_loop() {
	bool position_loaded = false;

	Position pos{};
	std::thread search_thread;

	std::string command;
	std::string command_type;
	std::vector<std::string> command_sections;
	while (std::getline(std::cin, command)) {
		if (command == "quit") {
			if (search_thread.joinable()) {
				search_thread.join();
			}
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
			position_loaded = false;
		}
		else if (command_type == "position") {
			if (searching) {
				std::cout << "Ignoring command: position" << std::endl;
			}
			else {
				position_loaded = uci_position_command(command_sections, pos);
			}
		}
		else if (command_type == "go") {
			if (searching || !position_loaded) {
				std::cout << "Ignoring command: go" << std::endl;
			}
			else {
				uci_go_command(command_sections, search_thread, pos);
			}
		}
		else if (command_type == "stop") {
			if (!searching) {
				std::cout << "Ignoring command: stop" << std::endl;
			}
			else {
				searching = false;
				search_thread.join();
			}
		}
		else {
			std::cout << "Unknown command: " << command << std::endl;
		}
	}
}

void uci_go_command(std::vector<std::string>& command_sections, std::thread& search_thread, Position& pos) {
	if (search_thread.joinable()) {
		search_thread.join();
	}
	search_thread = std::thread(get_best_move, std::ref(pos));
}

bool uci_position_command(std::vector<std::string>& command_sections, Position& pos) {
	if (command_sections[1] == "startpos") {
		load_from_fen(pos, start_fen);
	}
	else if (command_sections[1] == "fen") {
		std::string fen = "";
		for (int token = 2; token < 8; token++) {
			fen += command_sections[token];
			fen += " ";
		}
		load_from_fen(pos, fen);
	}
	else {
		std::cout << "Invalid command format for: position" << std::endl;
		return false;
	}
	return true;
}*/