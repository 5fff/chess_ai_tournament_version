#ifndef MINIMAX_H
#define MINIMAX_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <deque>
#include <vector>
#include <string>
#include "my_chess_board.h"
#include "my_helper_functions.h"
#include "common_var.h"
#include "move_generation_functions.h"
#include <chrono>

using namespace std;

class stop_watch
{
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
public:
  long check_time()//get time elapsed in ms
  {
    std::chrono::time_point<std::chrono::high_resolution_clock> t_now = std::chrono::high_resolution_clock::now();
    long time_span = chrono::duration_cast<std::chrono::milliseconds>(t_now-start_time).count();
    return time_span;
  }
  void start()
  {
    start_time = std::chrono::high_resolution_clock::now();
  }
};


//this is a string with builtin functionality to convert move_obj to string
struct state_id
{
  string key;
  void update(move_obj move)
  {
    key += move.from_square;
    key += move.to_square;
  }
};

//history table
//it is essentially a map wrapper, it indexes staes with strings of actions lead to this state
struct ht
{
  unordered_map<std::string,char> table;
  // what state(identifier) are you at, and what move are you looking up
  // state_id pass by value is necesary as I'm modifing and using it
  char lookup(state_id id, move_obj& move)
  {
    id.update(move);
    auto location = table.find(id.key);
    if(location != table.end())
    {
      //has entry
      return location -> second; //return the char value(used as a signed number)
    }
    else
    {
      //no such entry, so histry table value  = 0
      return 0;
    }
  }

  // what state(identifier) are you at, and what move are you incrementing
  void increment(state_id id, move_obj& move)
  {
    id.update(move);
    auto location = table.find(id.key);
    if(location != table.end())
    {
      //has entry
      if((location -> second) < 120)
      {
        //make max ht value capped at 120 so it does not over flow and cause undefined behavior
        (location -> second) = (location -> second) + 1; //increment entry
      }
    }
    else
    {
      //no such entry, so create new entry value  = 1
      table[id.key] = 1;
    }
    return;
  }
};


struct Minimax
{
  // Chess_Board board;//chess board with all the information
  ht history_table;
  move_obj best_move;
  short qs_depth_setting = 0; //default 0 for protection
  string selected_move_str;//this is the move selected by minimax, NOT VALID until get_move() is called
  Chess_Board current_chess_board;//this is a copy of the current board
  Chess_Board get_next_board();//return a Chess_Board object which represent the chess board after current move
  long time_limit;
  char root_playing_side;
  bool win_move_found_stop_searching = false;
  stop_watch rolex;
  //iterative deepening wrapper
  move_obj tl_id_minimax(Chess_Board board_in, char min_or_max);

  //depth limited minimax root node
  move_obj minimax_begin(Chess_Board board_in, short depth, char min_or_max);

  //depth limited minimax non root node
  // minimax_recursive is both min and max function, min_or_max = 1(max) or -1(min)
  short minimax_recursive(Chess_Board& board_in, state_id& my_state_id, short depth, short qs_depth, char min_or_max, short alpha, short beta);

  string get_move(string fen_str, three_fold_tracker& tf_tracker, long time_limit_in, short qs_depth_setting_in);

  vector<move_obj> order_moves(state_id& my_state_id, vector<move_obj>& move_list);
};


class timeout : public exception
{
  //private members
private:
  long time_stamp;
public:
  long get_time_stamp(){return time_stamp;}
  timeout(long t):time_stamp(t){}
};



#endif
