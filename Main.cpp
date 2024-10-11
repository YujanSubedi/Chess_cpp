#include "core_lib/global_values.hpp"
#include "core_lib/network_interface.hpp"
#include "core_lib/renderer.hpp"
#include "core_lib/minimax_bot.hpp"

int main(void){
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1200, 900, "Chess Window"); // (screenWidth, screenHeight, Title)
	SetTargetFPS(60);

	GAME_BOARD CHESS_GAME_BOARD;
	Chess_Renderer Graphics_Renderer;
	Minimax_bot Opponent_Bot(3);
	Network_interface Network_connection;

	bool Have_played_move= false;
	Piece_color User_player_color;
	Player_type User_player_type;
	MOVE_CLASS Opponent_move;
	MOVE_CLASS User_last_move;

	Rander_state Game_rander_state = Starting_state;

	while (!WindowShouldClose()) {
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		if(Game_rander_state == Starting_state){
			User_player_type = Graphics_Renderer.Handle_click_during_selcetion_mode();
			if(User_player_type == Playing_vs_bot || User_player_type == Playing_as_server){
				Game_rander_state = Select_color_state;
			}
			else if(User_player_type == Playing_as_client){
				char ip[] = "127.0.0.1";
				Network_connection.initiliaze_as_client(ip, 8888, User_player_color);
				Game_rander_state = Game_playing_state;
			}
		}

		else if(Game_rander_state == Select_color_state){
			User_player_color = Graphics_Renderer.Handle_click_during_color_section();
			if(User_player_color != not_selected){
				Game_rander_state = Game_playing_state;
				if(User_player_type == Playing_as_server){
					Network_connection.initiliaze_as_server(8888, User_player_color);
				}
			}
		}

		else if(Game_rander_state == Game_playing_state){
			if(Have_played_move && User_player_type != Playing_vs_bot){
				Network_connection.send_Move(User_last_move);
				Have_played_move = false;;
			}

			if(CHESS_GAME_BOARD.Get_turn() != User_player_color){
				if(Playing_vs_bot == User_player_type){
					Opponent_Bot.play_in_parallel(CHESS_GAME_BOARD);
				}
				else if( Network_connection.receive_Move(Opponent_move)){
					CHESS_GAME_BOARD.play_move(Opponent_move);
				}
			}
			else if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
				if(User_player_color == white_colored){
					Have_played_move = Graphics_Renderer.Handle_mouse_click_white(CHESS_GAME_BOARD, User_last_move);
				}
				else{
					Have_played_move = Graphics_Renderer.Handle_mouse_click_black(CHESS_GAME_BOARD, User_last_move);
				}
			}
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			if(Game_rander_state == Starting_state){
				Graphics_Renderer.Render_selcetion_mode();
			}

			else if(Game_rander_state== Select_color_state){
				Graphics_Renderer.Render_color_section();
			}

			else if(Game_rander_state == Game_playing_state){
				if(User_player_color == white_colored){
					Graphics_Renderer.Render_for_white(CHESS_GAME_BOARD);
				}
				else{
					Graphics_Renderer.Render_for_black(CHESS_GAME_BOARD);
				}
			}
		}
		EndDrawing();

		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();
}
