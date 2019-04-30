#include "minimax.h"
#include <limits.h>
// #define DEPTH_LIMIT 5
// #define DEPTH_LIMIT 4 // Takes about 15 seconds for every move
// #define DEPTH_LIMIT 3 // much faster
#define MAX 1
#define MIN -1
#define SAFE_DEPTH_LIMIT 999
#define MAX_ABS_VAL 9999
#define REPLACE_ABS_VAL 11111

short Minimax::minimax_recursive(Chess_Board& board_in, state_id& my_state_id, short depth, short qs_depth, char min_or_max, short alpha, short beta)
{
  long time_spent = rolex.check_time();
  if(time_spent > time_limit)
  {
    //we are out of time, throw timeout error
    throw(timeout(time_spent));
  }

  //min_or_max = 1 for max,white
  //min_or_max = -1 for min,black

  // base case
  //first check terminal cases and return terminal values

  //######## 50 moves ########
  //terminal value = 0 (draw)
  if(board_in.fifty_move_counter == 50) return 0;

  //######## three fold repetition ########
  //terminal value = 0 (draw)
  if(board_in.tf_tracker.is_three_fold()) return 0;

  //######## insufficient material ########
  //terminal value = 0 (draw)
  if(board_in.is_i_m()) return 0;

  //the next two terminal checks both require move generation, so generate move first

  //######## checkmated ########
  //doublect checkmate: king being checked and no legal moves available
  //terminal value
  // black being checkmated + 999
  // white being checkmated - 999


  //######## stale mate ########
  //detect checkmate: king not being checked and no legal moves available
  //terminal value = 0 (draw)

  //generate moves
  vector<move_obj> available_moves;
  generate_moves(available_moves,board_in);

  //check if king in check
  // unsigned char my_side = min_or_max == (-1) ? 'b':'w';
  unsigned char other_side = min_or_max == (-1) ? 'w':'b';
  unsigned char my_king_location = min_or_max == (-1) ? board_in.king_location[0]:board_in.king_location[1];

  if(available_moves.size()==0)
  {
    if(square_is_attacked(board_in, get_2d(my_king_location), other_side))
    {
      //######## checkmated ########
      //king in check, no moves available, I'm checkmated
      // black being checkmated + 999    -1 * -1 * 999 = 999
      // white being checkmated - 999    -1 * 1  * 999 = -999
      return (-min_or_max*MAX_ABS_VAL);
    }
    else
    {
      //######## stale mate ########
      //king not being checked but no legal moves available
      //stale mate, 0
      return 0;
    }
  }

  //if depth limited reached, return eval value
  if(depth > 0)
  {
    depth--;//for the recursive function input
  }
  else
  {
    //depth ==0
    if(qs_depth > 0 && nqs_test(board_in))
    {
      qs_depth --;
    }
    else return board_in.get_h(); //either run out of qs_depth or qs(not worth to keep searching)

  }

  //recursive part
  //for each available moves, get the new board state, ask next function for value
  // keep the max/min value depends on min_or_max

  // this value will be replaced by higher or lower value depends on min_or_max;
  // it will be replaced(or "replaced" by same 0) if program and game runs correctly because at this point there
  // will be moves if not returned at terminal conditions
  short best_value = REPLACE_ABS_VAL*(-min_or_max); //worst value for min or max that will be replaced
  move_obj best_move;

  //order moves by ht value below, higher ht value goes first
  available_moves = order_moves(my_state_id, available_moves);


  for(auto&& tmp_move : available_moves)
  {
    Chess_Board next_board(board_in);
    next_board.update(tmp_move); //get next board state
    state_id next_state_id(my_state_id);
    next_state_id.update(tmp_move); //this state_id can uniquely indentify a board state within the same rootnode.

    //get the min/max value,  notice "-min_or_max" flips the sign and color
    short tmp_value = minimax_recursive(next_board, next_state_id, depth, qs_depth, -min_or_max, alpha, beta);
    if(min_or_max*(tmp_value-best_value) >= 0)
    {
      //explanation:
      // when min_or_max = 1  (max, white)
      // if condition equivalent to (tmp_value > best_value), replace if find higher value
      // when min_or_max = -1 (min, black)
      // if condition equivalent to (tmp_value < best_value), replace if find lower value
      best_value = tmp_value;
      best_move = tmp_move;
    }
    switch(min_or_max)
    {
      case MIN: if(best_value <= alpha)
                {
                  history_table.increment(my_state_id, tmp_move);//increment ht value if pruned
                  return best_value;//fail low, prune
                }
                if(best_value < beta)beta=best_value;

                break;
      case MAX: if(best_value >= beta)
                {
                  history_table.increment(my_state_id, tmp_move);//increment ht value if pruned
                  return best_value;//fail high, prune
                }
                if(best_value > alpha)alpha=best_value;
                break;
    }
  }
  history_table.increment(my_state_id, best_move); //increment ht value for returned move(if not pruned)
  //note that if pruned the program will return before the last line, so it won't increment if it was pruned
  return best_value;
}


//minimax_begin does not detect terminal conditions because the game ends at terminal condition,
//thus minimax will never be called on a termional state.
//root node wil use the qs depth stored in minimax object, no need to pass in as it doesn't change
//Return: minimax_begin returns a move_obj (a move)
move_obj Minimax::minimax_begin(Chess_Board board_in, short depth, char min_or_max)
{
  vector<move_obj> available_moves;
  generate_moves(available_moves,board_in);
  move_obj best_move;
  short best_value = MAX_ABS_VAL*(-min_or_max); //worst value for min or max that will be replaced
  for(auto&& tmp_move : available_moves)
  {
    Chess_Board next_board(board_in);
    state_id next_state_id;
    next_board.update(tmp_move); //get next board state
    next_state_id.update(tmp_move); //this state_id can uniquely indentify a board state within the same rootnode.
    //get the min/max value,  notice "-min_or_max" flips the sign and color
    short tmp_value = minimax_recursive(next_board, next_state_id, depth - 1, qs_depth_setting, -min_or_max, -MAX_ABS_VAL, MAX_ABS_VAL);//last two arguments are alpha and beta
    if(min_or_max*(tmp_value-best_value) >= 0)
    {
      //explanation:
      // when min_or_max = 1  (max, white)
      // if condition equivalent to (tmp_value > best_value), replace if find higher value
      // when min_or_max = -1 (min, black)
      // if condition equivalent to (tmp_value < best_value), replace if find lower value
      best_value = tmp_value;
      best_move = tmp_move;
    }
    if(best_value == MAX_ABS_VAL*min_or_max)
    {
      //if I see a winning move, raise flag and stop searching
      win_move_found_stop_searching = true;
      cout<<"win move found = true, returning!"<<endl;
      return best_move;
    }
    //root node will never prune
  }
  return best_move;
}

// the point of iterative deepening is to have a best action within limited time,
// time_limit is a double variable that specify the time given for this calculation
//returns a move_obj
move_obj Minimax::tl_id_minimax(Chess_Board board_in, char min_or_max)
{
  //this move must have a valid solution, so force the depth 1
  cout<<"depth = 1"<<endl;

  long bk_time_limit = time_limit;
  time_limit = LONG_MAX;
  best_move = minimax_begin(board_in, 1, min_or_max);// this best move is the Minimax class variable
  time_limit = bk_time_limit;

  if(win_move_found_stop_searching)
  {
    cout<<"win move found, stop searching"<<endl;
    return best_move;
  }
  try
  {
    //for loop used as counter only, the only limit is the time so far;
    for(short d = 2; true; d++)
    {
      cout<<"depth = "<<d<<endl;
      best_move = minimax_begin(board_in, d, min_or_max);// this best move is the Minimax class variable
      if(win_move_found_stop_searching)
      {
        cout<<"win move found, stop searching"<<endl;
        return best_move;
      }
    }
  }
  catch(timeout err)
  {
    cout<<"timeout, returning result"<<endl;
  }

  return best_move;
}

//this function is called in the AI::get_move() function, it takes in a fen string
//and updated three_fold_tracker from the persistent object
//this function returns san string
string Minimax::get_move(string fen_str, three_fold_tracker& tf_tracker, long time_limit_in, short qs_depth_setting_in)
{
  time_limit = time_limit_in;
  qs_depth_setting = qs_depth_setting_in;
  rolex.start(); //start timer
  //cutoff_time is shared with all functions within the same minimax object
  Chess_Board board;
  board.load_fen(fen_str);//load board
  current_chess_board = board;
  board.tf_tracker = tf_tracker;// attach three fold repetiton information

  char min_or_max = board.side_to_move == 'b'? (-1):(1);
  move_obj move = tl_id_minimax(board, min_or_max);
  cout<<"Move Selected: "<<translate_to_san(move)<<endl;
  cout<<"Move Obj Dump:"<<(short)move.piece.data<<" "<<(short)move.promote_to.data<<" "<<(short)move.from_square<<" "<<(short)move.to_square<<" "<<(short)move.type<<endl;
  selected_move_str = translate_to_san(move);
  return selected_move_str;
}

//return a Chess_Board object which represent the chess board after current move
Chess_Board Minimax::get_next_board()
{
  Chess_Board next_chess_board = current_chess_board;
  next_chess_board.update(best_move);
  return next_chess_board;
}


vector<move_obj> Minimax::order_moves(state_id& my_state_id, vector<move_obj>& move_list)
{
  short move_size = move_list.size();
  vector<char> move_ht_val(move_size, 0); //a seperate vector to keep track of ht value
  char max_ht_val = 0;
  for(int i = 0;i< move_size; i++)
  {
    char tmp_ht_val = history_table.lookup(my_state_id, move_list[i]);
    move_ht_val[i] = tmp_ht_val;
    if(tmp_ht_val > max_ht_val)max_ht_val=tmp_ht_val; // get max
  }
  //I'm doing a sorting like bucket sort with know number of buckets
  vector<short> location(max_ht_val+1,0);//bucket end index, if max number is 5 then there needs 6 buckets from 0 to 5
  for(auto&& x: move_ht_val)location[x]++; //counting size of bucket
  location[0]--;//offset the first value by one, because if there are 5 numbers then the last index is 5-1 = 4
  for(short i = 1;i<location.size();i++)//if size ==1 then the loop will not run
  {
    location[i] = location[i] + location[i-1];//it's like cumulitave sum
  }
  //by now location[i] will point to the index where move with ht value=i should go. after each placement location[i] should -1

  move_obj error_move;
  error_move.from_square = 70;
  error_move.to_square = 77; //to trigger error detection
  vector<move_obj> sorted_moves(move_size,error_move);//emty vector with equal size
  for(short i = 0;i<move_size;i++)
  {
    char tmp_ht_val = move_ht_val[i];
    // sorted_moves[location[tmp_ht_val]] = move_list[i];//this is increasing order placement
    //to reverse place an element, reversed_index = size-1-normal_index, look at the line above, that is the normal order, and below is reversed order
    sorted_moves[move_size-1-location[tmp_ht_val]] = move_list[i];//this is reversed order,because I want higher ht value move go first
    location[tmp_ht_val] --; //decrement index by one for next placement
  }

  for(auto&& move : sorted_moves)
  {
    if(!(move.from_square >=0 && move.from_square <=63))
    {
      cout<<"ERROR!! After sort move got invalid move from square"<<endl;
      cout<<(short)move.piece.data<<" "<<(short)move.promote_to.data<<" "<<(short)move.from_square<<" "<<(short)move.to_square<<" "<<(short)move.type<<endl;
      throw;
    }
    if(!(move.to_square >=0 && move.to_square <=63))
    {
      cout<<"ERROR!! After sort move got invalid move to square"<<endl;
      cout<<(short)move.piece.data<<" "<<(short)move.promote_to.data<<" "<<(short)move.from_square<<" "<<(short)move.to_square<<" "<<(short)move.type<<endl;
      throw;
    }
  }
  return sorted_moves;


}
