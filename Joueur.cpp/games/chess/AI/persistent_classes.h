#ifndef PERSIS_C
#define PERSIS_C
// this file contain definitions for chess piece object and chess piece object
#include <iostream>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <string>
#include "my_chess_board.h"
#include "my_helper_functions.h"

using namespace std;

//this resides in AI class/object throughout the game, it keeps track of three fold repetition and
//keep the three_fold_tracker up to date
struct persistent_stfr_tracker
{
  three_fold_tracker tracker;
  vector<Chess_Board> chess_board_log; //keeps all the Chess_Board in history, this is for deriving opponents move
  bool update_before_move(string fen_str, string san_str); // this will be called before making a move to log opponent's move
  bool update_after_move(Chess_Board updated_board, move_obj move); // this will be called after making a move to log my own move
};

struct persistent_containier
{
  short qs_depth_limit = 2;// setting a default for protection
};
#endif
