#include "board.h"
#include "move.h"
#include "types.h"
#include <cstdint>
#include <string>

uint32_t flag_none		 = 0b00000000000000000000000;
uint32_t flag_pawn_start = 0b00100000000000000000000;
uint32_t flag_castle	 = 0b01000000000000000000000;
uint32_t flag_en_passant = 0b10000000000000000000000;

std::string get_move_str(uint32_t move) {
	uint32_t from_sq = move_from_sq(move);
	int from_rank = get_rank(from_sq);
	int from_file = get_file(from_sq);
	uint32_t to_sq = move_to_sq(move);
	int to_rank = get_rank(to_sq);
	int to_file = get_file(to_sq);

	std::string move_str;
	move_str.push_back('a' + from_file);
	move_str.push_back('0' + from_rank + 1);
	move_str.push_back('a' + to_file);
	move_str.push_back('0' + to_rank + 1);

	Piece promo_pce = move_promo_pce(move);

	if (static_cast<uint32_t>(promo_pce)) {
		move_str.push_back(pce_to_symbol(promo_pce));
	}

	return move_str;
}