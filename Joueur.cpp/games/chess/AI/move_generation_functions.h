#ifndef MOVE_GENERATION_FUNC_H
#define MOVE_GENERATION_FUNC_H

#include "my_helper_functions.h"
#include "common_var.h"
#include "my_chess_board.h"

using namespace std;

bool square_is_attacked(Chess_Board& board, pair<short,short> target_coord, char attacking_side);

//giving a square, return all moves(by given color) that attacks this piece,excluding en passant and castling
void who_can_attack_square(vector<move_obj>& available_moves, Chess_Board& board, pair<short,short> target_coord, char attacking_side);
void who_can_move_to_square(vector<move_obj>& available_moves, Chess_Board& board, pair<short,short> target_coord, char attacking_side);
string translate_to_san(move_obj input);
void generate_moves(vector<move_obj>& available_moves,Chess_Board& board);

#endif
