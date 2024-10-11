#include "global_values.hpp"
#include "game_bitboard.hpp"
#include <stdio.h>

void CHESS_BITBOARD::show_bitboard(unsigned long bit_board){
	for(int i=0; i<8; i++){
		printf("%8lb\n", ((bit_board)>>(8*(7-i))&0xff));
	}
	printf("\n");
}


CHESS_BITBOARD::CHESS_BITBOARD(): 
	whitepawn_mask(0x000000000000ff00),
	blackpawn_mask(0x00ff000000000000),
	leftmost_mask(0x7f7f7f7f7f7f7f7f),
	rightmost_mask(0xfefefefefefefefe),
	leftknight_mask(0x3f3f3f3f3f3f3f3f),
	rightknight_mask(0xfcfcfcfcfcfcfcfc)
{
	evaluation_of_piece = { 1, -1, 3, -3, 3, -3, 5, -5, 9, -9, 0, 0 };

	piece_bitboards[white_pawn] = 0xff00;
	piece_bitboards[black_pawn] = 0xff000000000000;
	piece_bitboards[white_knight] = 0x42;
	piece_bitboards[black_knight] = 0x4200000000000000;
	piece_bitboards[white_bishop] = 0x24;
	piece_bitboards[black_bishop] = 0x2400000000000000;
	piece_bitboards[white_rook] = 0x81;
	piece_bitboards[black_rook] = 0x8100000000000000;
	piece_bitboards[white_queen] = 0x10;
	piece_bitboards[black_queen] = 0x1000000000000000;
	piece_bitboards[white_king] = 0x8;
	piece_bitboards[black_king] = 0x800000000000000;

	white_castle_mask = 0x3;
	black_castle_mask = 0x3;

	white_ampason_mask = 0x0;
	black_ampason_mask = 0x0;

	update();
}

void CHESS_BITBOARD::update(){
	color_bitboards[0] = 0;
	color_bitboards[1] = 0;
	for(int i=0; i<6; i++){
		color_bitboards[0] |= piece_bitboards[2*i];
		color_bitboards[1] |= piece_bitboards[2*i+1];
	}
	occupied_bitboard = (color_bitboards[0])|(color_bitboards[1]);
	unoccupied_bitboard = ~occupied_bitboard;

	targets_bitboards[0] = pawn_targets(piece_bitboards[white_pawn], white_colored);
	targets_bitboards[1] = pawn_targets(piece_bitboards[black_pawn], black_colored);
	targets_bitboards[0] |= knight_moves(piece_bitboards[white_knight]);
	targets_bitboards[1] |= knight_moves(piece_bitboards[black_knight]);
	targets_bitboards[0] |= bishop_moves(piece_bitboards[white_bishop]);
	targets_bitboards[1] |= bishop_moves(piece_bitboards[black_bishop]);
	targets_bitboards[0] |= rook_moves(piece_bitboards[white_rook]);
	targets_bitboards[1] |= rook_moves(piece_bitboards[black_rook]);
	targets_bitboards[0] |= queen_moves(piece_bitboards[white_queen]);
	targets_bitboards[1] |= queen_moves(piece_bitboards[black_queen]);
	targets_bitboards[0] |= king_moves(piece_bitboards[white_king]);
	targets_bitboards[1] |= king_moves(piece_bitboards[black_king]);
}

int CHESS_BITBOARD::count_bits(unsigned long value){
	int count=0;
	while(value){
		value &= (value-1);
		count++;
	}
	return count;
}

void CHESS_BITBOARD::set_bit(piece_index piece_type, array<int, 2> position){
	unsigned long masking_bits=1;
	masking_bits <<= (position[0]*8 + position[1]);
	piece_bitboards[piece_type] = (piece_bitboards[piece_type])|(masking_bits);
}

void CHESS_BITBOARD::reset_bit(piece_index piece_type, array<int, 2> position){
	unsigned long masking_bits=1;
	masking_bits <<= (position[0]*8 + position[1]);
	piece_bitboards[piece_type] = (piece_bitboards[piece_type])&(~masking_bits);
}


unsigned long CHESS_BITBOARD::get_masking_bit(array<int, 2> position){
	unsigned long masking_bits=1;
	masking_bits <<= (position[0]*8 + position[1]);
	return  masking_bits;
}

vector<array<int, 2>> CHESS_BITBOARD::get_all_positions(piece_index Piece_type){
	vector<array<int, 2>> Positions_of_piece;
	unsigned long masking_bit=1;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			if(masking_bit&piece_bitboards[Piece_type]){
				// printf("%d %d\n", i, j);
				Positions_of_piece.push_back({i, j});
			}
			masking_bit <<= 1;
		}
	}
	return Positions_of_piece;
}

vector<array<int, 2>> CHESS_BITBOARD::get_bitboard_positions(unsigned long passed_bitboard){
	vector<array<int, 2>> Positions_of_piece;
	unsigned long masking_bit=1;
	for(int i=0; i<8; i++){
		for(int j=0; j<8; j++){
			if(masking_bit&passed_bitboard){
				// printf("%d %d\n", i, j);
				Positions_of_piece.push_back({i, j});
			}
			masking_bit <<= 1;
		}
	}
	return Positions_of_piece;
}

unsigned long CHESS_BITBOARD::get_colored_bitboard(Piece_color selected_color){
	return color_bitboards[selected_color];
}


int CHESS_BITBOARD::evalution_of_board(){
	int eval=0;
	for(int i=0; i<12; i++){
		eval += evaluation_of_piece[i]*count_bits(piece_bitboards[i]);
	}
	return eval;
}

unsigned long CHESS_BITBOARD::pawn_targets(unsigned long pawn_bitboard, Piece_color selected_color){
	unsigned long all_moves = 0;
	if(selected_color == white_colored){
		all_moves |= ((pawn_bitboard&leftmost_mask)<<9);
		all_moves |= ((pawn_bitboard&rightmost_mask)<<7);
	}
	else{
		all_moves |= ((pawn_bitboard&leftmost_mask)>>7);
		all_moves |= ((pawn_bitboard&rightmost_mask)>>9);
	}
	return all_moves;
}

unsigned long CHESS_BITBOARD::pawn_movements(unsigned long pawn_bitboard, Piece_color selected_color){
	unsigned long all_moves = 0;
	unsigned long cant_move_mask_1;
	unsigned long cant_move_mask_2;
	if(selected_color == white_colored){
		cant_move_mask_1 = unoccupied_bitboard>>8;
		cant_move_mask_2 = (unoccupied_bitboard>>16)&cant_move_mask_1;
		all_moves |= ((cant_move_mask_2&pawn_bitboard&whitepawn_mask)<<16);
		all_moves |= ((cant_move_mask_1&pawn_bitboard)<<8);
	}
	else{
		cant_move_mask_1 = unoccupied_bitboard<<8;
		cant_move_mask_2 = (unoccupied_bitboard<<16)&cant_move_mask_1;
		all_moves |= ((cant_move_mask_2&pawn_bitboard&blackpawn_mask)>>16);
		all_moves |= ((cant_move_mask_1&pawn_bitboard)>>8);
	}
	return all_moves;
}

unsigned long CHESS_BITBOARD::knight_moves(unsigned long knight_bitboard){
	unsigned long all_moves = 0;
	all_moves |= ((knight_bitboard&leftmost_mask)>>15);
	all_moves |= ((knight_bitboard&leftmost_mask)<<17);
	all_moves |= ((knight_bitboard&rightmost_mask)>>17);
	all_moves |= ((knight_bitboard&rightmost_mask)<<15);
	all_moves |= ((knight_bitboard&leftknight_mask)>>6);
	all_moves |= ((knight_bitboard&leftknight_mask)<<10);
	all_moves |= ((knight_bitboard&rightknight_mask)>>10);
	all_moves |= ((knight_bitboard&rightknight_mask)<<6);
	return all_moves;
}

unsigned long CHESS_BITBOARD::bishop_moves(unsigned long bishop_bitboard){
	unsigned long all_moves=0;
	unsigned long temp=bishop_bitboard;
	while(temp){
		temp &= leftmost_mask;
		temp <<= 9;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	temp=bishop_bitboard;
	while (temp) {
		temp &= leftmost_mask;
		temp >>= 7;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	temp=bishop_bitboard;
	while (temp) {
		temp &= rightmost_mask;
		temp <<= 7;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	temp=bishop_bitboard;
	while (temp) {
		temp &= rightmost_mask;
		temp >>= 9;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	return all_moves;
}

unsigned long CHESS_BITBOARD::rook_moves(unsigned long rook_bitboard){
	unsigned long all_moves=0;
	unsigned long temp=rook_bitboard;
	while(temp){
		temp &= leftmost_mask;
		temp <<= 1;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	temp=rook_bitboard;
	while (temp) {
		temp &= rightmost_mask;
		temp >>= 1;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	temp=rook_bitboard;
	while (temp) {
		temp >>= 8;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	temp=rook_bitboard;
	while (temp) {
		temp <<= 8;
		all_moves |= temp;
		temp &= unoccupied_bitboard;
	}
	return all_moves;
}

unsigned long CHESS_BITBOARD::queen_moves(unsigned long queen_bitboard){
	return rook_moves(queen_bitboard) | bishop_moves(queen_bitboard);
}

unsigned long CHESS_BITBOARD::king_moves(unsigned long king_bitboard){
	unsigned long all_moves = (king_bitboard>>8)|(king_bitboard<<8);
	unsigned long templeft = king_bitboard&leftmost_mask;
	unsigned long tempright = king_bitboard&rightmost_mask;
	all_moves |= (templeft<<1);
	all_moves |= (templeft>>7);
	all_moves |= (templeft<<9);
	all_moves |= (tempright>>1);
	all_moves |= (tempright>>9);
	all_moves |= (tempright<<7);
	return all_moves;
}

void CHESS_BITBOARD::update_move(MOVE_CLASS played_move){
	if(played_move.type == movement){
		reset_bit(played_move.initial_piece, played_move.initial_pos);
		set_bit(played_move.initial_piece, played_move.final_pos);
	}
	else if(played_move.type == capture){
		reset_bit(played_move.initial_piece, played_move.initial_pos);
		reset_bit(played_move.final_piece, played_move.final_pos);
		set_bit(played_move.initial_piece, played_move.final_pos);
	}
	else if(played_move.initial_piece==white_pawn || played_move.initial_piece==black_pawn){
		if(played_move.type == promotion){
			reset_bit(played_move.initial_piece, played_move.initial_pos);
			set_bit(played_move.promotion_piece, played_move.final_pos);
		}
		else if(played_move.type == promotion_with_capture){
			reset_bit(played_move.initial_piece, played_move.initial_pos);
			reset_bit(played_move.final_piece, played_move.final_pos);
			set_bit(played_move.promotion_piece, played_move.final_pos);
		}
	}
	update();
}

void CHESS_BITBOARD::undo_move(MOVE_CLASS played_move){
	if(played_move.type == movement){
		reset_bit(played_move.initial_piece, played_move.final_pos);
		set_bit(played_move.initial_piece, played_move.initial_pos);
	}
	else if(played_move.type == capture){
		reset_bit(played_move.initial_piece, played_move.final_pos);
		set_bit(played_move.initial_piece, played_move.initial_pos);
		set_bit(played_move.final_piece, played_move.final_pos);
	}
	else if(played_move.initial_piece==white_pawn || played_move.initial_piece==black_pawn){
		if(played_move.type == promotion){
			reset_bit(played_move.promotion_piece, played_move.final_pos);
			set_bit(played_move.initial_piece, played_move.initial_pos);
		}
		else if(played_move.type == promotion_with_capture){
			reset_bit(played_move.promotion_piece, played_move.final_pos);
			set_bit(played_move.initial_piece, played_move.initial_pos);
			set_bit(played_move.final_piece, played_move.final_pos);
		}
	}
	update();
}

piece_index CHESS_BITBOARD::Get_piece_type(array<int, 2> position){
	if(position[0]<0 || position[1]<0 || position[0]>7 || position[1]>7) return none_piece;
	unsigned long masking_bits=1;
	masking_bits <<= (position[0]*8 + position[1]);
	if(masking_bits & unoccupied_bitboard) return none_piece;
	for(int i=0; i<12; i++){
		if(masking_bits&piece_bitboards[i]){
			// printf("%d\n", i);
			return piece_index(i);
		}
	}
	return none_piece;
}

vector<MOVE_CLASS> CHESS_BITBOARD::get_moves(array<int, 2> position){
	vector<MOVE_CLASS> result_vector;
	piece_index selected_piece_type = Get_piece_type(position);
	if(selected_piece_type == none_piece) return result_vector;
	Piece_color Selected_color = (selected_piece_type&1)? black_colored: white_colored;
	Piece_color Opponent_color = (Selected_color==white_colored)? black_colored: white_colored;
	unsigned long position_mask=get_masking_bit(position);

	vector<array<int, 2>> temp_vector;
	unsigned long available_position_bitboard = unoccupied_bitboard;
	unsigned long capture_position_bitboard = color_bitboards[Opponent_color];

	if(selected_piece_type == white_pawn || selected_piece_type == black_pawn){
		available_position_bitboard &= pawn_movements(position_mask, Selected_color);
		capture_position_bitboard &= pawn_targets(position_mask, Selected_color);
		if((Selected_color==white_colored && position_mask&blackpawn_mask) || (Selected_color==black_colored && position_mask&whitepawn_mask)){
			temp_vector = get_bitboard_positions(available_position_bitboard);
			for(array<int, 2> next_position: temp_vector){
				MOVE_CLASS temp_move(position, next_position, selected_piece_type);
				temp_move.type = promotion;
				if(Selected_color == white_colored){
					temp_move.promotion_piece = white_knight;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = white_bishop;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = white_rook;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = white_queen;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
				}
				else if(Selected_color == black_colored){
					temp_move.promotion_piece = black_knight;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = black_bishop;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = black_rook;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = black_queen;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
				}
			}
			temp_vector = get_bitboard_positions(capture_position_bitboard);
			// printf("reached final pos %lx \n", capture_position_bitboard);
			for(array<int, 2> next_position: temp_vector){
				MOVE_CLASS temp_move(position, next_position, selected_piece_type);
				temp_move.type = promotion_with_capture;
				temp_move.final_piece = Get_piece_type(next_position);
				if(Selected_color == white_colored){
					temp_move.promotion_piece = white_knight;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = white_bishop;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = white_rook;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = white_queen;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
				}
				else if(Selected_color == black_colored){
					temp_move.promotion_piece = black_knight;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = black_bishop;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = black_rook;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
					temp_move.promotion_piece = black_queen;
					if(is_move_valid(temp_move)){
						result_vector.push_back(temp_move);
					}
				}
			}
		}
		else{
			temp_vector = get_bitboard_positions(available_position_bitboard);
			for(array<int, 2> next_position: temp_vector){
				MOVE_CLASS temp_move(position, next_position, selected_piece_type);
				temp_move.type = movement;
				if(is_move_valid(temp_move)){
					result_vector.push_back(temp_move);
				}
			}
			temp_vector = get_bitboard_positions(capture_position_bitboard);
			for(array<int, 2> next_position: temp_vector){
				MOVE_CLASS temp_move(position, next_position, selected_piece_type);
				temp_move.type = capture;
				temp_move.final_piece = Get_piece_type(next_position);
				if(is_move_valid(temp_move)){
					result_vector.push_back(temp_move);
				}
			}
		}
		return result_vector;
	}

	unsigned long temp_bitboard=0;
	if(selected_piece_type == white_knight || selected_piece_type == black_knight){
		temp_bitboard =  knight_moves(position_mask);
	}
	else if(selected_piece_type == white_bishop || selected_piece_type == black_bishop){
		temp_bitboard =  bishop_moves(position_mask);
	}
	else if(selected_piece_type == white_rook || selected_piece_type == black_rook){
		temp_bitboard =  rook_moves(position_mask);
	}
	else if(selected_piece_type == white_queen || selected_piece_type == black_queen){
		temp_bitboard =  queen_moves(position_mask);
	}
	else if(selected_piece_type == white_king || selected_piece_type == black_king){
		temp_bitboard =  king_moves(position_mask);
	}

	available_position_bitboard &= temp_bitboard;
	capture_position_bitboard &= temp_bitboard;
	temp_vector = get_bitboard_positions(available_position_bitboard);
	for(array<int, 2> next_position: temp_vector){
		MOVE_CLASS temp_move(position, next_position, selected_piece_type);
		temp_move.type = movement;
		if(is_move_valid(temp_move)){
			result_vector.push_back(temp_move);
		}
	}
	temp_vector = get_bitboard_positions(capture_position_bitboard);
	for(array<int, 2> next_position: temp_vector){
		MOVE_CLASS temp_move(position, next_position, selected_piece_type);
		temp_move.type = capture;
		temp_move.final_piece = Get_piece_type(next_position);
		if(is_move_valid(temp_move)){
			result_vector.push_back(temp_move);
		}
	} return result_vector;
}


Game_state CHESS_BITBOARD::Is_checked(){
	bool white_is_checked =  targets_bitboards[1]&piece_bitboards[white_king];
	bool black_is_checked =  targets_bitboards[0]&piece_bitboards[black_king];
	if(white_is_checked && black_is_checked){
		return Invalid_state;
	}
	if(white_is_checked){
		//show_bitboard(targets_bitboards[1]);
		return White_checked;
	}
	if(black_is_checked){
		return Black_checked;
	}
	return Playing_on;
}

bool CHESS_BITBOARD::is_move_valid(MOVE_CLASS given_move){
	update_move(given_move);
	Game_state next_game_state = Is_checked();
	Piece_color player_color = given_move.initial_piece&1? black_colored: white_colored;
	undo_move(given_move);
	if((next_game_state == Invalid_state) || (next_game_state == Black_checked && player_color==black_colored) || ( next_game_state==White_checked && player_color==white_colored)){
		return false;
	}
	return true;
}

