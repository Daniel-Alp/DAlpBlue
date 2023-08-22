#include "board.h"
#include "move.h"
#include "makemove.h"
#include "movegen.h"
#include "parser.h"
#include "search.h"
#include "types.h"
#include "uci.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>


//The UCI is "blind" and does not validate commands. That is the responsibility of the GUI.
void uci_loop() {
	Position pos{};
	std::thread search_thread;

	std::string cmd;
	std::string cmd_type;
	std::vector<std::string> cmd_sections;

	while (std::getline(std::cin, cmd)) {
		cmd_sections = split_string(cmd, ' ');
		cmd_type = cmd_sections[0];

		if (cmd_type == "quit") {
			if (search_thread.joinable()) {
				search_thread.join();
			}
			break;
		}
		else if (cmd_type == "uci") {
			std::cout << "id name DAlpBlue" << std::endl;
			std::cout << "id author Daniel Alp" << std::endl;
			std::cout << "uciok" << std::endl;
		}
		else if (cmd_type == "isready") {
			std::cout << "readyok" << std::endl;
		}
		else if (cmd_type == "ucinewgame") {
			load_from_fen(pos, start_fen);
			//TODO: CLEAR TTable
		}
		else if (cmd_type == "position") {
			uci_position_command(cmd_sections, pos);
		}
		else if (cmd_type == "go") {
			if (search_thread.joinable()) {
				search_thread.join();
			}
			uci_go_command(cmd_sections, search_thread, pos);
		}
		else if (cmd_type == "stop") {
			searching = false;
			search_thread.join();
		}
		else {
			std::cout << "Unknown command: " << cmd << std::endl;
		}
	}
}

void uci_go_command(std::vector<std::string>& cmd_sections, std::thread& search_thread, Position& pos) {
	search_thread = std::thread(get_best_move, std::ref(pos));
}

void uci_position_command(std::vector<std::string>& cmd_sections, Position& pos) {
	int move_token = 0;

	if (cmd_sections[1] == "startpos") {
		load_from_fen(pos, start_fen);
		move_token = 3;
	}
	else {
		std::string fen = "";
		for (int token = 2; token < 8; token++) {
			fen += cmd_sections[token];
			fen += " ";
		}
		load_from_fen(pos, fen);
		move_token = 9;
	}

	while (move_token < cmd_sections.size()) {
		std::array<uint32_t, max_moves> pseudo_moves;
		int num_pseudo_moves;
		gen_pseudo_moves(pos, pseudo_moves, num_pseudo_moves, false);

		for (int i = 0; i < num_pseudo_moves; i++) {
			uint32_t move = pseudo_moves[i];
			if (get_move_str(move) == cmd_sections[move_token]) {
				make_move(pos, move);
			}
		}
		move_token++;
	}
}