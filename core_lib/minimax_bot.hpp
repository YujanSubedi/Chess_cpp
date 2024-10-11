#ifndef MINIMAX_BOT_HPP
#define MINIMAX_BOT_HPP

#include "game_board.hpp"
#include "global_values.hpp"

class Minimax_bot{
private:
	unsigned short MAX_DEPTH;
	unsigned int total_itteration;
	bool can_create_thread_for_bot;
	bool can_close_thread_used_by_bot;
public:
	Minimax_bot(unsigned short);
	int minimax_evalutaion(GAME_BOARD Given_board, MOVE_CLASS simulated_move, int alpha_value, int beta_value, unsigned depth);
	MOVE_CLASS get_best_move(GAME_BOARD Given_board);
	void play_in_parallel(GAME_BOARD &CHESS_GAME_BOARD);
};

#endif
