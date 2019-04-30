// #include "common_var.h"
// #include "my_helper_functions.h"
#include "my_chess_board.h"
#include "my_helper_functions.h"
#include "common_var.h"
#include <bitset>


void three_fold_tracker::update(move_obj move)
{
  // three fold repetition can only be tested or evaluate as true if there are >= 8 moves in history
  //the built in deque only needs to store recent 8 moves.

  //first if the new input move is capture or pawn moves(include promotion), then clear the queue
  // also SPECIAL NOTE: if it's a castling move, it will clear the queue as well
  // BECAUSE a castling move can happen at most once for each side, so a castling move will never
  // repeat in a given game(in another word, castling move will never equal to another move
  // in the move history), thus as long as a castling move is in the recent 8 move history,
  // the Simplified Three Fold repetiton will not be ture. So clearing the queue upon a castling
  //move is justified.
  if((move.piece.type() == PAWN)
    || move.type == M_CAP
    || move.type == M_BKC
    || move.type == M_BQC
    || move.type == M_WKC
    || move.type == M_WQC)
  {
    recent_moves.clear();
    if(debug_flag)
    {
      cout<<"! - ! - ! - ! - ! - ! -"<<endl;
      cout<<"Three Fold Tracker CLEARED, see move type(last number)"<<endl;
      print_raw_move(move);
      cout<<"! - ! - ! - ! - ! - ! -"<<endl;
    }
  }
  else
  {
    recent_moves.push_front(move);
  }
  //keep the deque size smaller than 8
  if(recent_moves.size()>8)
  {
    recent_moves.pop_back();
    if(recent_moves.size()>8)
    {
      cout<<"###### ERROR recent move deque size > 8 after pop!!!!!! "<<endl;
      throw;
    }
  }

  if(recent_moves.size()<8)
  {
    three_fold = false;
    return;
  }
  else // size is 8
  {
    for(short i = 0; i<4;i++)
    {
      if(!(recent_moves[i] == recent_moves[i+4]))
      {
        three_fold = false;
        return;
      }
    }
    // then all the pairs are equal, trigger three fold
    three_fold = true;
    // cout<<"Three Fold Detected!"<<endl;
    return;
  }
}

// update() needed to be called before getting the three fold flag
bool three_fold_tracker::is_three_fold()
{
  return three_fold;
}

void three_fold_tracker::print_raw_record()
{
  cout<<"##### TF Tracker Record #####"<<endl;
  for(auto&& move: recent_moves)
  {
    print_raw_move(move);
  }
  if(recent_moves.empty())cout<<"TF Tracker EMPTY!"<<endl;
  cout<<"#####     -------       #####"<<endl;
}

//part of the code is copied from provided prettyFEN function
void Chess_Board::load_fen(string fen)
{
    auto split = string_split(fen, ' ');
    auto first = split[0]; // the first part is always the board locations
    auto lines = string_split(first, '/');
    string castling_info = split[2];
    string en_passant_string = split[3];
    fifty_move_counter = stoi(split[4]);//get 50 move counter as integer (unsigned char)
    map<char,unsigned char> bw = {{'b',BLACK},{'w',WHITE}};
    side_to_move = split[1][0];
    int square_index = 0;
    for (int l = 7;l>=0;l--)
    {
        for (char &character : lines[l])
        {
            int asInt = (int)character - 48; // test for char '1' -> int 1
            if (asInt > -1 && asInt < 9)
            {
                // blank tiles n times, so repeat the . that many times
                square_index+= asInt; //skip blank squares
            }
            else // pieces
            {
                // it is a regular character for a piece
                square[square_index].assign(char2code[character]);
                if(character == 'k') king_location[0] = square_index;
                if(character == 'K') king_location[1] = square_index;
                square_index++;
            }
        }
    }
    //set casteling availabilty for kings and rooks
    for(char c : castling_info)
    {
        castling_availability |= castling_flags[c]; //set castling flag
    }
    //get en passant info
    ep_index = en_passant_string == "-"? -1 : SAN_1D[en_passant_string];
}

void Chess_Board::print_raw()
{
  for(short row=7;row>=0;row--)
  {
    for(short col=0;col<8;col++)
    {
      bitset<8> bit_format(square[get_1d(row,col)].data);
      cout<<bit_format<<" ";
    }
    cout<<endl;
  }
  bitset<8> bit_format(castling_availability);
  cout<<"kqKQ---- ep_index   side_to_move"<<endl<<
  bit_format<<"    "<<(int)ep_index<<"          "<<side_to_move<<endl<<
  "_____________________\n"<<endl;
}

//
void Chess_Board::update(move_obj &move)
{
  // #### Update last_move_capture flag ###
  // if last move is a capture, set flag to true, otherwise set to false
  last_move_capture = (move.type == M_CAP);


  if(!(move.from_square >=0 && move.from_square <=63))
  {
    cout<<"ERROR!! board update got invalid move from square"<<endl;
    cout<<(short)move.piece.data<<" "<<(short)move.promote_to.data<<" "<<(short)move.from_square<<" "<<(short)move.to_square<<" "<<(short)move.type<<endl;
    throw;
  }
  if(!(move.to_square >=0 && move.to_square <=63))
  {
    cout<<"ERROR!! board update got invalid move to square"<<endl;
    cout<<(short)move.piece.data<<" "<<(short)move.promote_to.data<<" "<<(short)move.from_square<<" "<<(short)move.to_square<<" "<<(short)move.type<<endl;
    throw;
  }
  // cout<<"board.update() start"<<endl;
  char last_side_to_move = side_to_move;
  //update or clear 50 move rule counter
  if(move.type == M_CAP || move.piece.type() == PAWN)
  {
    fifty_move_counter = 0;
  }
  else
  {
    fifty_move_counter ++ ;
  }

  //update three_fold repetition tracker
  tf_tracker.update(move);

  side_to_move = side_to_move == 'b'? 'w':'b';
  if(move.type == M_EP)
  {
    if(ep_index<=0)cout<<"ERROR!!!!!!!! INVALID EN PASSANT INDEX"<<endl;
    //clear the attacked square
    square[ep_index].clear();
    square[move.to_square].assign(square[move.from_square]);
    square[move.from_square].clear();
    unsigned char ep_target_index = last_side_to_move == 'b'? (ep_index + 8):(ep_index - 8);
    square[ep_target_index].clear();
    ep_index = -1;
    return;
  }
  ep_index = -1; //clear en pasant index
  //for all capture and regular move, just move the piece to destination and remove old location
  if(move.type == M_REG || move.type == M_CAP)
  {
    if(move.piece.type()==KING)// clear castling flag
    {
      if(last_side_to_move == 'b')
      {
        //reset flags
        king_location[0] = move.to_square;
        castling_availability &= (~(BLACK_KING_CASTLING_FLAG | BLACK_QUEEN_CASTLING_FLAG));
      }
      else
      {
        king_location[1] = move.to_square;
        castling_availability &= (~(WHITE_KING_CASTLING_FLAG | WHITE_QUEEN_CASTLING_FLAG));
      }
    }
    if(move.piece.type()==ROOK)//if rook moves then unset castling flag
    {
      switch(move.from_square)
      {
        case 0: castling_availability &= (~(WHITE_QUEEN_CASTLING_FLAG));
              break;
        case 7: castling_availability &= (~(WHITE_KING_CASTLING_FLAG));
              break;
        case 56: castling_availability &= (~(BLACK_QUEEN_CASTLING_FLAG));
              break;
        case 63: castling_availability &= (~(BLACK_KING_CASTLING_FLAG));
              break;
      }
    }
    if(move.promote_to.data != EMPTY)
    {
      square[move.to_square].assign(move.promote_to);
    }
    else
    {
      square[move.to_square].assign(square[move.from_square]);
    }
    square[move.from_square].clear();
    return;
  }

  if(move.type == M_JJ)
  {
    square[move.to_square].assign(square[move.from_square]);
    square[move.from_square].clear();
    ep_index = (char)move.to_square;
    return;
  }

  // precalculated positions:
  // castling type  king from    king to      rook from     rook to
  // BQC              60           58              56         59
  // BKC              60           62              63         61
  // WQC              4            2                0         3
  // WKC              4            6                7         5
  if(move.type == M_BQC)
  {
     //move king
     square[58].assign(square[60]);
     square[60].clear();
     king_location[0] = 58;
     //move rook
     square[59].assign(square[56]);
     square[56].clear();

     castling_availability &= (~(BLACK_QUEEN_CASTLING_FLAG));
     return;
  }
  if(move.type == M_BKC)
  {
     //move king
     square[62].assign(square[60]);
     square[60].clear();
     king_location[0] = 62;
     //move rook
     square[61].assign(square[63]);
     square[63].clear();

     castling_availability &= (~(BLACK_KING_CASTLING_FLAG));
     return;
  }
  if(move.type == M_WQC)
  {
     //move king
     square[2].assign(square[4]);
     square[4].clear();
     king_location[0] = 2;
     //move rook
     square[3].assign(square[0]);
     square[0].clear();

     castling_availability &= (~(WHITE_QUEEN_CASTLING_FLAG));
     return;
  }
  if(move.type == M_WKC)
  {
     //move king
     square[6].assign(square[4]);
     square[4].clear();
     king_location[0] = 6;
     //move rook
     square[5].assign(square[7]);
     square[7].clear();

     castling_availability &= (~(WHITE_KING_CASTLING_FLAG));
     return;
  }

  // cout<<"board.update() finish"<<endl;
}


void Chess_Board::print_board()
{
  for(short row=7;row>=0;row--)
  {
    for(short col=0;col<8;col++)
    {
      cout<<code2char[square[get_1d(row,col)].data]<<" ";
    }
    cout<<endl;
  }
  bitset<8> bit_format(castling_availability);
  cout<<"kqKQ---- ep_index   side_to_move   k\tK   "<<endl<<
  bit_format<<"    "<<(int)ep_index<<"          "<<side_to_move<<"\t    "<<(short)king_location[0]<<"\t"<<(short)king_location[1]<<endl<<
  "_____________________\n"<<endl;
  return;
}

short Chess_Board::get_h()
{
  // cout<<"get_h() called"<<endl;
  short h_val = 0;
  //simply sum all the material values, value mapping see common_var.h
  for(short i=0;i<64;i++)
  {
    h_val += piece2value[square[i].data];
  }
  // cout<<"get_h() finished"<<endl;
  return h_val;
}

bool Chess_Board::is_i_m()
{
  //according to server rule:
  // Insufficient material (K vs. K, K vs. KB, or K vs. KN)
  // knight, king bishop counter
  // if there's anything other than K,B or N then automatically return false

  //c++ switch case requres case being constant at compile time, so I have to list them here
  const unsigned char  KNIGHT      =  0b00001000;
  const unsigned char  BISHOP      =  0b00010000;
  const unsigned char  KING        =  0b00101000;
  const unsigned char  BLACK       =  0b10000000;
  const unsigned char  WHITE       =  0b11000000;
  const unsigned char  EMPTY       =  0b00000000;

  char b_b=0,w_b=0,w_n=0,b_n=0; //piece counters
  for(short i=0;i<64;i++)
  {
    switch(square[i].data)
    {
      case EMPTY:
          break;//skip empty
      case (BLACK|KING):
          break;//skip king
      case (WHITE|KING):
          break;//skip king
      case (BLACK|BISHOP):
          b_b++;
          break;
      case (WHITE|BISHOP):
          w_b++;
          break;
      case (BLACK|KNIGHT):
          b_n++;
          break;
      case (WHITE|KNIGHT):
          w_n++;
          break;
      default:
          return false;
          //if there's any other piece, then there are sufficient material
    };
  }
  //case by case checking below
  //according to server rule:
  // Insufficient material (K vs. K, K vs. KB, or K vs. KN)

  // K vs. K,  which all counter are 0
  if(b_b == 0 && w_b == 0 && w_n == 0 && b_n == 0) return true;

  // K vs. KB
  if((b_b == 0 && b_n == 0 && w_b == 1 && w_n == 0)||(b_b == 1 && b_n == 0 && w_b == 0 && w_n == 0)) return true;

  // K vs. KN
  if((b_b == 0 && b_n == 0 && w_b == 0 && w_n == 1)||(b_b == 0 && b_n == 1 && w_b == 0 && w_n == 0)) return true;

  return false;
}
