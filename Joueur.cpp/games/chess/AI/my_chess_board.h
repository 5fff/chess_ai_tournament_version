#ifndef MCB_HPP
#define MCB_HPP
// this file contain definitions for chess piece object and chess piece object
#include <iostream>
#include <string>
#include <map>
#include <deque>
#include "common_var.h"

// #include "my_helper_functions.h"

using namespace std;


// my_chess_piece is  used in chess board
// My_Chess_Piece is just a unsigned char with bunch of builtin functions
struct Chess_Piece
{
  unsigned char data=0; // 1 byte raw data
  unsigned char type() {return (data & TYPE_MASK);} //extract type code
  bool color_is(unsigned char color_code) {return ((data & COLOR_MASK) == color_code);} //true if color match, else false
  bool not_empty(){return (data != EMPTY);}
  bool is(unsigned char mask){return data == mask;}
  bool is_empty(){return (data == EMPTY);}
  void clear(){data = EMPTY;} // remove piece
  void assign(unsigned char target){data = target;} //unsigned char is smaller than pointer
  void assign(Chess_Piece target){data = target.data;} //overload
};




struct move_obj
{
  Chess_Piece piece, promote_to;
  unsigned char from_square = 0, to_square = 0, type;
  // type: 0 = Regular Move
  // type: 1 = Capture Move
  // type: 2 = En Passant Capture
  // type: 3 = k black king side castle
  // type: 4 = q black queen side castle
  // type: 5 = K white king side castle
  // type: 6 = Q white queen side castle
  // type: 7 = Pawn Double Jump

  // constructor (chess_piece, from, to, move_type)
  move_obj(){promote_to.assign(EMPTY);}
  //the comparison operator does not compare promote_to
  // bool operator==(move_obj& rhs) const
  // {
  //   return ((piece.data == rhs.piece.data)
  //           &&(from_square == rhs.from_square)
  //           &&(to_square == rhs.to_square)
  //           &&(type == rhs.type));
  // }
  bool operator==(move_obj& rhs) const
  {
    return ((from_square == rhs.from_square)
            &&(to_square == rhs.to_square)
            );
  }
  move_obj(Chess_Piece a, unsigned char b,unsigned char c, unsigned char d)
  {
    promote_to.data = 0;
    piece = a;
    from_square = b;
    to_square = c;
    type = d;
  }
};

//three fold tracker is a struct/object that contains the recent move history and
// can return three_fold status upon function request
// needed to be updated in every new board state by feeding move_obj
// this tracker is designed to be equipped in every Chess_Board object
struct three_fold_tracker
{
  bool debug_flag = false;
  deque<move_obj> recent_moves;// a deque to track recent moves
  bool three_fold = false;
  void update(move_obj move);
  void print_raw_record();
  bool is_three_fold(); //return true if three fold repetion condition true
};

//Chess_Node don't keep track of which side to move, it is simply a state, MiniMax recursive function will keep track of it
struct Chess_Board
{
  Chess_Piece square[64]; //64 squares on board, always
  //king location tracker
  // king_location[0] = black king
  // king_location[1] = white king

  unsigned char king_location[2];
  unsigned char fifty_move_counter = 0; //50 move rule counter
  three_fold_tracker tf_tracker;
  char side_to_move;
  unsigned char castling_availability = 0;
  char ep_index = -1; // No en passant available by default
  void load_fen(string fen);
  void print_raw();
  void print_board();
  void update(move_obj &move);
  short get_h();//get simple material evaluation h value

  bool is_i_m();//insufficient material

  bool last_move_capture = false;
};


#endif
