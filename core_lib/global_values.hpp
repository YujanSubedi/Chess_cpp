#ifndef GLOBAL_VLAUES_HPP
#define GLOBAL_VLAUES_HPP

#include <array>
#include <vector>
using std::array;
using std::vector;

enum piece_index {
  white_pawn,
  black_pawn,
  white_knight,
  black_knight,
  white_bishop,
  black_bishop,
  white_rook,
  black_rook,
  white_queen,
  black_queen,
  white_king,
  black_king,
  none_piece,
};

enum Piece_color { white_colored, black_colored, not_selected };

enum MOVE_TYPE { movement, capture, promotion, promotion_with_capture };

enum Game_state {
  White_checked,
  Black_checked,
  White_wins,
  Black_wins,
  Draw,
  Playing_on,
  Invalid_state
};

enum Player_type {
  Not_selected_mode,
  Playing_vs_bot,
  Playing_as_server,
  Playing_as_client
};

enum Rander_state {
  Starting_state,
  Select_color_state,
  Game_playing_state,
  Game_ended_state
};

struct MOVE_CLASS {
public:
  array<int, 2> initial_pos;
  array<int, 2> final_pos;
  piece_index initial_piece;
  piece_index final_piece;
  piece_index promotion_piece;
  MOVE_TYPE type;
  MOVE_CLASS() {}
  MOVE_CLASS(array<int, 2> pos_1, array<int, 2> pos_2,
             piece_index moved_piece) {
    initial_pos = pos_1;
    final_pos = pos_2;
    initial_piece = moved_piece;
  }
};

#endif
