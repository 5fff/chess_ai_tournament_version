#include "persistent_classes.h"

bool persistent_stfr_tracker::update_before_move(string fen_str, string san_str)
{
  Chess_Board tmp_board;
  tmp_board.load_fen(fen_str);
  chess_board_log.push_back(tmp_board);
  if(chess_board_log.size()>=2)//you can only have a move when you have two board states
  {
    //by now the log size >=2
    move_obj tmp_move = derive_move_obj(chess_board_log[chess_board_log.size()-2],chess_board_log.back(),san_str);

    // cout<<"Derive Move Result: ";
    // print_raw_move(tmp_move);
    // cout<<endl;


    // tracker.debug_flag = true;
    tracker.update(tmp_move); //now I have an updated persistent three_fold_tracker
    // tracker.debug_flag = false;
    // tracker.print_raw_record();
  }
  return true;
}

bool persistent_stfr_tracker::update_after_move(Chess_Board updated_board, move_obj move)
{
  chess_board_log.push_back(updated_board);
  // tracker.debug_flag = true;
  tracker.update(move);
  // tracker.debug_flag = false;
  // tracker.print_raw_record();
}
