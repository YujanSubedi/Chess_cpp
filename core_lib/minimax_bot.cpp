#include "global_values.hpp"
#include "game_board.hpp"
#include "minimax_bot.hpp"
#include <future>

Minimax_bot::Minimax_bot(unsigned short up_depth){
	MAX_DEPTH = up_depth;
	can_create_thread_for_bot = true;
	can_close_thread_used_by_bot = false;
}

int Minimax_bot::minimax_evalutaion(GAME_BOARD Given_board, MOVE_CLASS simulated_move,int alpha_value, int beta_value, unsigned depth){
	Given_board.play_move(simulated_move);
	Piece_color selected_color  = Given_board.Get_turn();
	int best_eval = selected_color==white_colored? -1000: 1000;
	if(depth > MAX_DEPTH){
		best_eval = Given_board.Get_evalutaion_of_the_game();
		Given_board.undo_lastmove(simulated_move);
		total_itteration++;
		return best_eval;
	}
	vector<MOVE_CLASS> Possible_moves_by_bot = Given_board.get_all_available_moves(Given_board.Get_turn());
	int eval;
	for(MOVE_CLASS temp_move: Possible_moves_by_bot){
		eval = minimax_evalutaion(Given_board, temp_move, alpha_value, beta_value, depth+1);
		if(selected_color==white_colored){
			if(eval > best_eval){
				best_eval = eval;
				alpha_value = best_eval;
			}
		}
		else{
			if (eval<best_eval){
				best_eval = eval;
				beta_value = best_eval;
			}
		}
		if(alpha_value>=beta_value){
			break;
		}
	}
	Given_board.undo_lastmove(simulated_move);
	return best_eval;
}


MOVE_CLASS Minimax_bot::get_best_move(GAME_BOARD Given_board){
	total_itteration = 0;
	vector<std::future<int>> future_evaluations;
	vector<MOVE_CLASS> Possible_moves_by_bot = Given_board.get_all_available_moves(Given_board.Get_turn());
	printf("The no of aviable moves are %ld \n", Possible_moves_by_bot.size());
	MOVE_CLASS best_move = Possible_moves_by_bot[0];
	Piece_color selected_color  = Given_board.Get_turn();
	int best_eval = selected_color==white_colored? -1000: 1000;
	int eval;
	for(MOVE_CLASS temp_move: Possible_moves_by_bot){
		future_evaluations.push_back(std::async(std::launch::async, &Minimax_bot::minimax_evalutaion, this, Given_board, temp_move, -1000, 1000, 0));
		//eval = minimax_evalutaion(Given_board, temp_move, 0);
	}
	for(int i=0; i<Possible_moves_by_bot.size(); i++){
		eval = future_evaluations[i].get();
		if((selected_color==white_colored && eval>best_eval) || (selected_color==black_colored && eval<best_eval)){
			best_eval =eval;
			best_move = Possible_moves_by_bot[i];
		}
	}
	printf("The best evalation is %d \n", best_eval);
	can_close_thread_used_by_bot= true;
	printf("total itteration %u \n", total_itteration);
	return best_move;
}

void Minimax_bot::play_in_parallel(GAME_BOARD &CHESS_GAME_BOARD){
	static std::future<MOVE_CLASS> Future_bot_move;
	if(can_create_thread_for_bot){
		Future_bot_move = std::async(std::launch::async, &Minimax_bot::get_best_move, this, CHESS_GAME_BOARD);
		can_create_thread_for_bot = false;
		//(&GAME_BOARD::play_move, &CHESS_GAME_BOARD, MOVE_BY_BOT(CHESS_GAME_BOARD));
	}
	else if(can_close_thread_used_by_bot){
		CHESS_GAME_BOARD.play_move(Future_bot_move.get());
		can_create_thread_for_bot= true;
		can_close_thread_used_by_bot= false;
	}
}
