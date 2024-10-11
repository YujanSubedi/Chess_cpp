#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "game_bitboard.hpp"

class GAME_BOARD{
private:
	CHESS_BITBOARD CURRENT_GAME_BITBOARD;
	Piece_color Current_turn_to_play;

public:
	GAME_BOARD();

	Piece_color Get_turn();
	int Get_evalutaion_of_the_game();
	piece_index get_piece_at_position(array<int, 2> position);
	vector<array<int, 2>> get_Pieces_positions(piece_index piece_type);

	void play_move(MOVE_CLASS played_move);
	void undo_lastmove(MOVE_CLASS played_move);
	Game_state get_game_state();

	vector<MOVE_CLASS> get_available_moves(array<int, 2> position);
	vector<MOVE_CLASS> get_all_available_moves(Piece_color Selected_color);
};

#endif
