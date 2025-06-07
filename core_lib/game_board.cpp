#include "game_board.hpp"

GAME_BOARD::GAME_BOARD() { Current_turn_to_play = white_colored; }

Piece_color GAME_BOARD::Get_turn() { return Current_turn_to_play; }

int GAME_BOARD::Get_evalutaion_of_the_game() {
  return CURRENT_GAME_BITBOARD.evalution_of_board();
}

piece_index GAME_BOARD::get_piece_at_position(array<int, 2> position) {
  return CURRENT_GAME_BITBOARD.Get_piece_type(position);
}

vector<array<int, 2>> GAME_BOARD::get_Pieces_positions(piece_index piece_type) {
  return CURRENT_GAME_BITBOARD.get_all_positions(piece_type);
  // return
  // CURRENT_GAME_BITBOARD.get_bitboard_positions(CURRENT_GAME_BITBOARD.pawn_movements(0xff00,
  // white_colored));
}

void GAME_BOARD::play_move(MOVE_CLASS played_move) {
  CURRENT_GAME_BITBOARD.update_move(played_move);
  Current_turn_to_play =
      Current_turn_to_play == white_colored ? black_colored : white_colored;
}

void GAME_BOARD::undo_lastmove(MOVE_CLASS played_move) {
  CURRENT_GAME_BITBOARD.undo_move(played_move);
  Current_turn_to_play =
      Current_turn_to_play == white_colored ? black_colored : white_colored;
}

vector<MOVE_CLASS> GAME_BOARD::get_available_moves(array<int, 2> position) {
  return CURRENT_GAME_BITBOARD.get_moves(position);
}

vector<MOVE_CLASS>
GAME_BOARD::get_all_available_moves(Piece_color Selected_color) {
  vector<MOVE_CLASS> resuting_vector;
  vector<MOVE_CLASS> temp;
  vector<array<int, 2>> all_positions =
      CURRENT_GAME_BITBOARD.get_bitboard_positions(
          CURRENT_GAME_BITBOARD.get_colored_bitboard(Selected_color));
  for (array<int, 2> seleceted_position : all_positions) {
    temp = get_available_moves(seleceted_position);
    resuting_vector.insert(resuting_vector.end(), temp.begin(), temp.end());
  }
  return resuting_vector;
}

Game_state GAME_BOARD::get_game_state() { return Playing_on; }
