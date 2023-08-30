#include "board.h"
#include "move.h"
#include "makemove.h"
#include "movegen.h"
#include "parser.h"
#include "search.h"
#include "timemanagement.h"
#include "transposition.h"
#include "types.h"
#include "uci.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <vector>


//The UCI is "blind" and does not validate commands. That is the responsibility of the GUI.
void uci_loop() {
	Position pos{};
	SearchData search_data{};
	search_data.nodes = 0;
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
			pos = load_from_fen(start_fen);
			clr_hash_table();
		}
		else if (cmd_type == "position") {
			uci_position_command(cmd_sections, pos);
		}
		else if (cmd_type == "go") {
			if (search_thread.joinable()) {
				search_thread.join();
			}
			uci_go_command(cmd_sections, search_thread, search_data, pos);
		}
		else if (cmd_type == "stop") {
			search_data.searching = false;
			if (search_thread.joinable()) {
				search_thread.join();
			}
		}
		else {
			std::cout << "Unknown command: " << cmd << std::endl;
		}
	}
}

void uci_go_command(std::vector<std::string>& cmd_sections, std::thread& search_thread, SearchData& search_data, Position& pos) {
	int32_t wtime = 0;
	int32_t btime = 0;
	int32_t winc = 0;
	int32_t binc = 0;
	int32_t moves_to_go = 0;

	std::string token_type;
	
	for (int token = 1; token < cmd_sections.size() - 1; token += 2) {
		token_type = cmd_sections[token];
		if (token_type == "wtime") {
			wtime = std::stoi(cmd_sections[token + 1]);
		}
		else if (token_type == "btime") {
			btime = std::stoi(cmd_sections[token + 1]);
		}
		else if (token_type == "winc") {
			winc = std::stoi(cmd_sections[token + 1]);
		}
		else if (token_type == "binc") {
			binc = std::stoi(cmd_sections[token + 1]);
		}
		else if (token_type == "movestogo") {
			moves_to_go = std::stoi(cmd_sections[token + 1]);
		}
	}

	int32_t player_time = 0;
	int32_t opp_time = 0;
	int32_t player_inc = 0;
	int32_t opp_inc = 0;

	if (pos.side_to_move == Color::WHITE) {
		player_time = wtime;
		opp_time = btime;
		player_inc = winc;
		opp_inc = binc;
	}
	else {
		player_time = btime;
		opp_time = wtime;
		player_inc = binc;
		opp_inc = winc;
	}
	search_data.start_time = get_current_time();
	search_data.time_allotted = get_time_allotted(player_time, opp_time, player_inc, opp_inc, moves_to_go);
	search_thread = std::thread(get_best_move, std::ref(pos), std::ref(search_data));
}

void uci_position_command(std::vector<std::string>& cmd_sections, Position& pos) {
	int move_token = 0;

	if (cmd_sections[1] == "startpos") {
		pos = load_from_fen(start_fen);
		move_token = 3;
	}
	else {
		std::string fen = "";
		for (int token = 2; token < 8; token++) {
			fen += cmd_sections[token];
			fen += " ";
		}
		pos = load_from_fen(fen);
		move_token = 9;
	}

	std::array<uint32_t, max_moves> pseudo_moves;
	int num_pseudo_moves;	
	while (move_token < cmd_sections.size()) {
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