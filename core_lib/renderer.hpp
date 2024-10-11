#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "game_board.hpp"
#include <raylib.h>

class Chess_Renderer{
private:
	static const int x_offset;
	static const int y_offset;
	static const int size_of_square;
	Texture2D Pieces_textures[12];
	Texture2D BOARD_TEXTURE;
	Texture2D REVERDED_BOARD_TEXTURE;
	Piece_color clicked_color;
	Piece_color selected_color;
	piece_index clicked_piece;
	piece_index selected_piece;
	array<int, 2> clicked_pos;
	array<int, 2> selected_pos;
	vector<MOVE_CLASS> temp_move;
public:
	Chess_Renderer();
	void Render_selcetion_mode();
	void Render_color_section();
	void Render_for_white(GAME_BOARD CHESS_GAME_BOARD);
	void Render_for_black(GAME_BOARD CHESS_GAME_BOARD);
	Player_type Handle_click_during_selcetion_mode();
	Piece_color Handle_click_during_color_section();
	bool Handle_mouse_click_white(GAME_BOARD &CHESS_GAME_BOARD, MOVE_CLASS &User_move);
	bool Handle_mouse_click_black(GAME_BOARD &CHESS_GAME_BOARD, MOVE_CLASS &User_move);
};

#endif
