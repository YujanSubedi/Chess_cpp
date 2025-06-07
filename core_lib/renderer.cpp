#include "renderer.hpp"
#include "global_values.hpp"
#include <raylib.h>

const int Chess_Renderer::x_offset = 80;
const int Chess_Renderer::y_offset = 80;
const int Chess_Renderer::size_of_square = 80;

Chess_Renderer::Chess_Renderer() {
  Pieces_textures[0] = LoadTexture("./pics/WP.png");
  Pieces_textures[1] = LoadTexture("./pics/BP.png");
  Pieces_textures[2] = LoadTexture("./pics/WN.png");
  Pieces_textures[3] = LoadTexture("./pics/BN.png");
  Pieces_textures[4] = LoadTexture("./pics/WB.png");
  Pieces_textures[5] = LoadTexture("./pics/BB.png");
  Pieces_textures[6] = LoadTexture("./pics/WR.png");
  Pieces_textures[7] = LoadTexture("./pics/BR.png");
  Pieces_textures[8] = LoadTexture("./pics/WQ.png");
  Pieces_textures[9] = LoadTexture("./pics/BQ.png");
  Pieces_textures[10] = LoadTexture("./pics/WK.png");
  Pieces_textures[11] = LoadTexture("./pics/BK.png");
  BOARD_TEXTURE = LoadTexture("./pics/board.png");
  REVERDED_BOARD_TEXTURE = LoadTexture("./pics/rboard.png");
  clicked_color = not_selected;
  selected_color = not_selected;
  clicked_piece = none_piece;
  selected_piece = none_piece;
}

void Chess_Renderer::Render_selcetion_mode() {
  DrawText("Press A to host the game", 300, 200, 20, BLACK);
  DrawText("Press B to join the game", 300, 300, 20, BLACK);
  DrawText("Press C to play with bot", 300, 400, 20, BLACK);
}

void Chess_Renderer::Render_color_section() {
  DrawText("Press A to play as white", 300, 250, 20, BLACK);
  DrawText("Press B to play as black", 300, 350, 20, BLACK);
}

Player_type Chess_Renderer::Handle_click_during_selcetion_mode() {
  if (IsKeyPressed(KEY_A)) {
    return Playing_as_server;
  } else if (IsKeyPressed(KEY_B)) {
    return Playing_as_client;
  } else if (IsKeyPressed(KEY_C)) {
    return Playing_vs_bot;
  }
  return Not_selected_mode;
}

Piece_color Chess_Renderer::Handle_click_during_color_section() {
  if (IsKeyPressed(KEY_A)) {
    return white_colored;
  } else if (IsKeyPressed(KEY_B)) {
    return black_colored;
  }
  return not_selected;
}

void Chess_Renderer::Render_for_white(GAME_BOARD CHESS_GAME_BOARD) {
  DrawTexture(BOARD_TEXTURE, 0, 0, WHITE);
  if (selected_piece != none_piece) {
    DrawRectangle(82 + selected_pos[1] * 80, 82 + (7 - selected_pos[0]) * 80,
                  78, 78, (Color){30, 150, 30, 255});
    for (MOVE_CLASS highligeted_move : temp_move) {
      if (highligeted_move.type == movement ||
          highligeted_move.type == promotion) {
        DrawRectangle(82 + highligeted_move.final_pos[1] * 80,
                      82 + (7 - highligeted_move.final_pos[0]) * 80, 78, 78,
                      (Color){30, 150, 150, 128});
      } else if (highligeted_move.type == capture ||
                 highligeted_move.type == promotion_with_capture) {
        DrawRectangle(82 + highligeted_move.final_pos[1] * 80,
                      82 + (7 - highligeted_move.final_pos[0]) * 80, 78, 78,
                      (Color){150, 30, 30, 128});
      }
    }
  }
  for (int i = 0; i < 12; i++) {
    for (array<int, 2> piece_position :
         CHESS_GAME_BOARD.get_Pieces_positions(piece_index(i))) {
      DrawTexture(Pieces_textures[i],
                  x_offset + size_of_square * piece_position[1],
                  y_offset + size_of_square * (7 - piece_position[0]), GRAY);
    }
  }
}

void Chess_Renderer::Render_for_black(GAME_BOARD CHESS_GAME_BOARD) {
  DrawTexture(REVERDED_BOARD_TEXTURE, 0, 0, WHITE);
  if (selected_piece != none_piece) {
    DrawRectangle(82 + (7 - selected_pos[1]) * 80, 82 + (selected_pos[0]) * 80,
                  78, 78, (Color){30, 150, 30, 255});
    for (MOVE_CLASS highligeted_move : temp_move) {
      if (highligeted_move.type == movement ||
          highligeted_move.type == promotion) {
        DrawRectangle(82 + (7 - highligeted_move.final_pos[1]) * 80,
                      82 + (highligeted_move.final_pos[0]) * 80, 78, 78,
                      (Color){30, 150, 150, 128});
      } else if (highligeted_move.type == capture ||
                 highligeted_move.type == promotion_with_capture) {
        DrawRectangle(82 + (7 - highligeted_move.final_pos[1]) * 80,
                      82 + (highligeted_move.final_pos[0]) * 80, 78, 78,
                      (Color){150, 30, 30, 128});
      }
    }
  }
  for (int i = 0; i < 12; i++) {
    for (array<int, 2> piece_position :
         CHESS_GAME_BOARD.get_Pieces_positions(piece_index(i))) {
      DrawTexture(Pieces_textures[i],
                  x_offset + size_of_square * (7 - piece_position[1]),
                  y_offset + size_of_square * (piece_position[0]), GRAY);
    }
  }
}

bool Chess_Renderer::Handle_mouse_click_white(GAME_BOARD &CHESS_GAME_BOARD,
                                              MOVE_CLASS &User_move) {
  clicked_pos[0] = (8 - GetMouseY() / 80);
  clicked_pos[1] = GetMouseX() / 80 - 1;
  if ((clicked_pos[0] >= 0 && clicked_pos[0] < 8) &&
      (clicked_pos[1] >= 0 && clicked_pos[1] < 8)) {
    clicked_piece = CHESS_GAME_BOARD.get_piece_at_position(clicked_pos);
    if (clicked_piece == none_piece) {
      clicked_color = not_selected;
    } else {
      clicked_color = clicked_piece % 2 == 1 ? black_colored : white_colored;
      if (selected_piece == none_piece && clicked_color == black_colored) {
        return false;
      }
    }
    if (selected_pos == clicked_pos) {
      selected_piece = none_piece;
      selected_pos = {-1, -1};
    } else if (selected_piece == none_piece ||
               (clicked_color == selected_color)) {
      selected_pos = clicked_pos;
      selected_piece = clicked_piece;
      selected_color = clicked_color;
      temp_move = CHESS_GAME_BOARD.get_available_moves(selected_pos);
    } else if (selected_piece != none_piece) {
      for (MOVE_CLASS available_move : temp_move) {
        if (clicked_pos == available_move.final_pos) {
          CHESS_GAME_BOARD.play_move(available_move);
          User_move = available_move;
          selected_piece = none_piece;
          selected_pos = {-1, -1};
          return true;
        }
      }
    }
  } else {
    selected_piece = none_piece;
    selected_pos = {-1, -1};
  }
  return false;
}

bool Chess_Renderer::Handle_mouse_click_black(GAME_BOARD &CHESS_GAME_BOARD,
                                              MOVE_CLASS &User_move) {
  clicked_pos[0] = GetMouseY() / 80 - 1;
  clicked_pos[1] = (8 - GetMouseX() / 80);
  if ((clicked_pos[0] >= 0 && clicked_pos[0] < 8) &&
      (clicked_pos[1] >= 0 && clicked_pos[1] < 8)) {
    clicked_piece = CHESS_GAME_BOARD.get_piece_at_position(clicked_pos);
    if (clicked_piece == none_piece) {
      clicked_color = not_selected;
    } else {
      clicked_color = clicked_piece % 2 == 1 ? black_colored : white_colored;
      if (selected_piece == none_piece && clicked_color == white_colored) {
        return false;
      }
    }
    if (selected_pos == clicked_pos) {
      selected_piece = none_piece;
      selected_pos = {-1, -1};
    } else if (selected_piece == none_piece ||
               (clicked_color == selected_color)) {
      selected_pos = clicked_pos;
      selected_piece = clicked_piece;
      selected_color = clicked_color;
      temp_move = CHESS_GAME_BOARD.get_available_moves(selected_pos);
    } else if (selected_piece != none_piece) {
      for (MOVE_CLASS available_move : temp_move) {
        if (clicked_pos == available_move.final_pos) {
          CHESS_GAME_BOARD.play_move(available_move);
          User_move = available_move;
          selected_piece = none_piece;
          selected_pos = {-1, -1};
          return true;
        }
      }
    }
  } else {
    selected_piece = none_piece;
    selected_pos = {-1, -1};
  }
  return false;
}
