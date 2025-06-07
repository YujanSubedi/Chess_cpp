#ifndef GAME_BITBOARD_HPP
#define GAME_BITBOARD_HPP

#include "global_values.hpp"
class CHESS_BITBOARD {
private:
  unsigned long piece_bitboards[12];
  unsigned long color_bitboards[2];
  unsigned long targets_bitboards[2];
  unsigned long occupied_bitboard;
  unsigned long unoccupied_bitboard;

  unsigned long white_ampason_mask;
  unsigned long black_ampason_mask;
  unsigned short white_castle_mask;
  unsigned short black_castle_mask;

  const unsigned long whitepawn_mask;
  const unsigned long blackpawn_mask;
  const unsigned long leftmost_mask;
  const unsigned long rightmost_mask;
  const unsigned long leftknight_mask;
  const unsigned long rightknight_mask;

  array<int, 12> evaluation_of_piece;

  int count_bits(unsigned long value);
  void set_bit(piece_index piece_type, array<int, 2> position);
  void reset_bit(piece_index piece_type, array<int, 2> position);
  void update();

public:
  CHESS_BITBOARD();

  unsigned long get_masking_bit(array<int, 2> position);
  unsigned long get_colored_bitboard(Piece_color selected_color);

  void show_bitboard(unsigned long bit_board);

  unsigned long pawn_targets(unsigned long pawn_bitboard,
                             Piece_color selected_color);
  unsigned long pawn_movements(unsigned long pawn_bitboard,
                               Piece_color selected_color);
  unsigned long knight_moves(unsigned long knight_bitboard);
  unsigned long bishop_moves(unsigned long bishop_bitboard);
  unsigned long rook_moves(unsigned long rook_bitboard);
  unsigned long queen_moves(unsigned long queen_bitboard);
  unsigned long king_moves(unsigned long king_bitboard);

  int evalution_of_board();
  Game_state Is_checked();
  bool is_move_valid(MOVE_CLASS given_move);
  void update_move(MOVE_CLASS played_move);
  void undo_move(MOVE_CLASS played_move);

  piece_index Get_piece_type(array<int, 2> position);
  vector<array<int, 2>> get_all_positions(piece_index Piece_type);
  vector<array<int, 2>> get_bitboard_positions(unsigned long passed_bitboard);
  vector<MOVE_CLASS> get_moves(array<int, 2> position);
};

#endif
