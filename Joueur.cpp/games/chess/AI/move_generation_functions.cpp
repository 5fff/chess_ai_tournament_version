#include "move_generation_functions.h"
#include <array>
#include <sstream>
#define ROW 0
#define COL 1
bool square_is_attacked(Chess_Board& board, pair<short,short> target_coord,char attacking_side)
{
  pair<short,short> tmp_coord, direction;
  unsigned char attacking_color;
  if(attacking_side == 'w')
  {
    // attacked_color = BLACK;
    attacking_color = WHITE;
  }
  else
  {
    // attacked_color = WHITE;
    attacking_color = BLACK;
  }
  // 3 2 1
  // 4 X 0
  // 5 6 7
  //first check for sliding pieces
  //check for ENEMY queen,rook on right up, left, down directions 0,2,4,6
  // cout<<"## 1"<<endl;
  for(short i : {0,2,4,6})
  {
    direction = DIRECTION[i];
    tmp_coord = target_coord; //reset coordinate
    tmp_coord = tmp_coord + direction;
    while(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.not_empty())
      {
        if(current_square.is(attacking_color|QUEEN)
        || current_square.is(attacking_color|ROOK))
        {
          return true;
        }
        else break;
      }
      else tmp_coord = tmp_coord + direction;
    }
  }

  // cout<<"## 5"<<endl;
  //check for ENEMY queen,bishop on four diagnal directions 1,3,5,7
  for(short i : {1,3,5,7})
  {
    direction = DIRECTION[i];
    tmp_coord = target_coord; //reset coordinate
    tmp_coord = tmp_coord + direction;
    while(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.not_empty())
      {
        if(current_square.is(attacking_color|QUEEN)
        || current_square.is(attacking_color|BISHOP))
        {
          return true;
        }
        else break;
      }
      else tmp_coord = tmp_coord + direction;
    }
  }
  // cout<<"## 10"<<endl;
  //check for knight
  for(int i=0;i<8;i++)
  {
    tmp_coord = target_coord + HORSE_VECTOR[i];
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)
      && current_square.type()==KNIGHT)
      {
        return true;
      }
    }
  }
  //check for enemy pawn on two direction, calculate direction for different side
  pair<short,short> pawn_vec[2][2]={{{-1,-1},{-1,1}},{{1,1},{1,-1}}};
  short pawn_vec_index = attacking_side == 'w'? 0:1;
  for(auto vector : pawn_vec[pawn_vec_index])
  {
    tmp_coord = target_coord + vector;
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)&& current_square.type()==PAWN)
      {
        return true;
      }
    }
  }
  //pawn double jump cannot attack, so ignore double jump in attacked check

  //check for enemy king
  for(int i=0;i<8;i++)
  {
    tmp_coord = target_coord + DIRECTION[i];
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)
      && current_square.type()==KING)
      {
        return true;
      }
    }
  }
  return false;
}
// #############################################################################
// #############################################################################
// #############################################################################

// function return which piece can attack this square, input: board, target coordnate, who's attacking
void who_can_attack_square(vector<move_obj>& available_moves, Chess_Board& board, pair<short,short> target_coord, char attacking_side)
{
  // vector<move_obj> available_moves;//empty vector
  pair<short,short> tmp_coord, direction;
  unsigned char attacking_color;
  if(attacking_side == 'w')
  {
    // attacked_color = BLACK;
    attacking_color = WHITE;
  }
  else
  {
    // attacked_color = WHITE;
    attacking_color = BLACK;
  }
  // 3 2 1
  // 4 X 0
  // 5 6 7
  //first check for sliding pieces
  //check for ENEMY queen,rook on right up, left, down directions 0,2,4,6
  // cout<<"## 1"<<endl;
  for(short i : {0,2,4,6})
  {
    direction = DIRECTION[i];
    tmp_coord = target_coord; //reset coordinate
    tmp_coord = tmp_coord + direction;
    while(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.not_empty())
      {
        if(current_square.is(attacking_color|QUEEN)
        || current_square.is(attacking_color|ROOK))
        {
          available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_CAP));
          break;
        }
        else break;
      }
      else tmp_coord = tmp_coord + direction;
    }
  }

  // cout<<"## 5"<<endl;
  //check for ENEMY queen,bishop on four diagnal directions 1,3,5,7
  for(short i : {1,3,5,7})
  {
    direction = DIRECTION[i];
    tmp_coord = target_coord; //reset coordinate
    tmp_coord = tmp_coord + direction;
    while(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.not_empty())
      {
        if(current_square.is(attacking_color|QUEEN)
        || current_square.is(attacking_color|BISHOP))
        {
          available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_CAP));
          break;
        }
        else break;
      }
      else tmp_coord = tmp_coord + direction;
    }
  }
  // cout<<"## 10"<<endl;
  //check for knight
  for(int i=0;i<8;i++)
  {
    tmp_coord = target_coord + HORSE_VECTOR[i];
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)
      && current_square.type()==KNIGHT)
      {
        available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_CAP));
      }
    }
  }
  //check for enemy pawn on two direction, calculate direction for different side
  pair<short,short> pawn_vec[2][2]={{{-1,-1},{-1,1}},{{1,1},{1,-1}}};
  short pawn_vec_index = attacking_side == 'w'? 0:1;
  for(auto vector : pawn_vec[pawn_vec_index])
  {
    tmp_coord = target_coord + vector;
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)&& current_square.type()==PAWN)
      {
        available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_CAP));
      }
    }
  }
  //if this square is on special two lines where pawns can double jump to, then check for far pawn
  //WAIT! pawn can not capture with double jump, skipped

  //check for enemy king
  for(int i=0;i<8;i++)
  {
    tmp_coord = target_coord + DIRECTION[i];
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)
      && current_square.type()==KING)
      {
        available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_CAP));
        break;
      }
    }
  }
  return;
}

// ########################################################################
// ########################################################################
// ########################################################################
//this function return who can move to a square NOTE: move is different than attack, in the case of pawn
void who_can_move_to_square(vector<move_obj>& available_moves,Chess_Board& board, pair<short,short> target_coord, char attacking_side)
{
  // vector<move_obj> available_moves;//empty vector
  pair<short,short> tmp_coord, direction;
  unsigned char attacking_color;
  if(attacking_side == 'w')
  {
    // attacked_color = BLACK;
    attacking_color = WHITE;
  }
  else
  {
    // attacked_color = WHITE;
    attacking_color = BLACK;
  }
  // 3 2 1
  // 4 X 0
  // 5 6 7
  //first check for sliding pieces
  //check for ENEMY queen,rook on right up, left, down directions 0,2,4,6
  // cout<<"## 1"<<endl;
  for(short i : {0,2,4,6})
  {
    direction = DIRECTION[i];
    tmp_coord = target_coord; //reset coordinate
    tmp_coord = tmp_coord + direction;
    while(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.not_empty())
      {
        if(current_square.is(attacking_color|QUEEN)
        || current_square.is(attacking_color|ROOK))
        {
          available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_REG));
          break;
        }
        else break;
      }
      else tmp_coord = tmp_coord + direction;
    }
  }

  // cout<<"## 5"<<endl;
  //check for ENEMY queen,bishop on four diagnal directions 1,3,5,7
  for(short i : {1,3,5,7})
  {
    direction = DIRECTION[i];
    tmp_coord = target_coord; //reset coordinate
    tmp_coord = tmp_coord + direction;
    while(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.not_empty())
      {
        if(current_square.is(attacking_color|QUEEN)
        || current_square.is(attacking_color|BISHOP))
        {
          available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_REG));
          break;
        }
        else break;
      }
      else tmp_coord = tmp_coord + direction;
    }
  }
  // cout<<"## 10"<<endl;
  //check for knight
  for(int i=0;i<8;i++)
  {
    tmp_coord = target_coord + HORSE_VECTOR[i];
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)
      && current_square.type()==KNIGHT)
      {
        available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_REG));
      }
    }
  }
// pawn regular move
  if(attacking_side == 'w')
  {
    //white pawn can only move up, so no need to chek destination cell  lower than 3rd rank (index 2)
    if( (get<ROW>(target_coord) >=2 ) && board.square[get_1d(target_coord+DOWN)].data == (WHITE|PAWN))
    {
      available_moves.push_back(move_obj(
        board.square[get_1d(target_coord+DOWN)],
        get_1d(target_coord+DOWN),
        get_1d(target_coord),
        M_REG));
    }
  }
  else
  {
    //black pawn can only move downp, so no need to chek destination cell higher than 6th rank (index 5)
    if( (get<ROW>(target_coord) <= 5) &&board.square[get_1d(target_coord+UP)].data == (BLACK|PAWN))
    {
      available_moves.push_back(move_obj(
        board.square[get_1d(target_coord+UP)],
        get_1d(target_coord+UP),
        get_1d(target_coord),
        M_REG));
    }
  }

  //if this square is on special two lines where pawns can double jump to, then check for far pawn
  if(attacking_side == 'w' && get<ROW>(target_coord) == 3)
  {
    //downward check
    if(board.square[get_1d(target_coord+DOWN)].data == EMPTY
    && board.square[get_1d(target_coord+DOWN2)].data == (WHITE|PAWN))
    {
      available_moves.push_back(move_obj(
        board.square[get_1d(target_coord+DOWN2)],
        get_1d(target_coord+DOWN2),
        get_1d(target_coord),
        M_JJ));
    }
  }
  else if(attacking_side == 'b' && get<ROW>(target_coord) == 4)
  {
    //upward check
    if(board.square[get_1d(target_coord+UP)].data == EMPTY
    && board.square[get_1d(target_coord+UP2)].data == (BLACK|PAWN))
    {
      available_moves.push_back(move_obj(
        board.square[get_1d(target_coord+UP2)],
        get_1d(target_coord+UP2),
        get_1d(target_coord),
        M_JJ));
    }
  }

  //check for enemy king
  for(int i=0;i<8;i++)
  {
    tmp_coord = target_coord + DIRECTION[i];
    if(is_bounded(tmp_coord))
    {
      Chess_Piece current_square = board.square[get_1d(tmp_coord)];
      if(current_square.color_is(attacking_color)
      && current_square.type()==KING)
      {
        available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_REG));
        break;
      }
    }
  }
  return;
}

string translate_to_san(move_obj input)
{
  stringstream san_string;
  // type: 0 = Regular Move
  // type: 1 = Capture Move
  // type: 2 = En Passant Capture
  // type: 3 = k black king side castle
  // type: 4 = q black queen side castle
  // type: 5 = K white king side castle
  // type: 6 = Q white queen side castle
  // type 7 double jumo
  if(input.type <= 2 || (input.type==M_JJ))
  {
    // // ##DEBUG###
    // if(!(input.from_square >=0 && input.from_square <=63))
    // {
    //   cout<<"ERROR!! translate,invalid from_square"<<endl;
    //   cout<<(short)input.piece.data<<" "<<(short)input.promote_to.data<<" "<<(short)input.from_square<<" "<<(short)input.to_square<<" "<<(short)input.type<<endl;
    //   throw;
    // }
    // if(!(input.to_square >=0 && input.to_square <=63))
    // {
    //   cout<<"ERROR!! translate,invalid to_square"<<endl;
    //   cout<<(short)input.piece.data<<" "<<(short)input.promote_to.data<<" "<<(short)input.from_square<<" "<<(short)input.to_square<<" "<<(short)input.type<<endl;
    //   throw;
    // }

    // ##########
    char piece_char = code2char[input.piece.data];//get corresponding char
    if(piece_char != 'P' && piece_char != 'p')
    {
      san_string<<piece_char;
    }
    //then add from location
    san_string<<SAN_TABLE[input.from_square];
    if(input.type == M_CAP || input.type == M_EP)san_string<<'x';
    //then add to_location
    san_string<<SAN_TABLE[input.to_square];
    //promotion is detected by checking ptomote_to variable, if == EMPTY, then not promote
    if(input.promote_to.data != EMPTY && input.piece.type() == PAWN)san_string<<code2char[input.promote_to.data];
    //if(input.type == M_EP)san_string<<"e.p.";
    return san_string.str();
  }
  if(input.type == 3 ||input.type == 5)
  {
    return "O-O";
  }
  if(input.type == 4 ||input.type == 6)
  {
    return "O-O-O";
  }
  cout<<"translate_to_san ERROR, search for AHEIF"<<endl;
  throw;
}


void generate_moves(vector<move_obj>& available_moves,Chess_Board& board)
{
  // generating moves without thinking about king safety, later remove moves that will result in king being checked
  // vector<move_obj> available_moves;
  char side_to_move = board.side_to_move;
  // cout<<"Generating Moves, side to move ="<<side_to_move<<endl;
  char other_side;
  unsigned char moving_side_color;
  if(side_to_move == 'b')
  {
    other_side = 'w';
    moving_side_color = BLACK;
    // other_side_color = WHITE;
  }else
  {
    other_side = 'b';
    moving_side_color = WHITE;
    // other_side_color = BLACK;
  }
  for(short row=0; row<8;row++)
  {
    for(short col=0;col<8;col++)
    {
      if(board.square[get_1d({row,col})].color_is(moving_side_color)) continue; //can't move to own piece
      if(board.square[get_1d({row,col})].is_empty())
      {
        who_can_move_to_square(available_moves, board, {row,col}, side_to_move);
      }
      else // else it will be other side's pieces, so who can attack?
      {
        who_can_attack_square(available_moves, board, {row,col}, side_to_move);
      }

    }
  }
  // generate en passant

  if(board.ep_index != -1)
  {
    pair<short,short> target_coord = (get_2d(board.ep_index));
    pair<short,short> pawn_vec[2][2]={{{-1,-1},{-1,1}},{{1,1},{1,-1}}};
    short pawn_vec_index = side_to_move == 'w'? 0:1;
    for(auto vector : pawn_vec[pawn_vec_index])
    {
      pair<short,short> tmp_coord = target_coord + vector;
      if(is_bounded(tmp_coord))
      {
        Chess_Piece current_square = board.square[get_1d(tmp_coord)];
        if(current_square.color_is(moving_side_color)&& current_square.type()==PAWN)
        {
          // cout<<"########## en passant x1 ############"<<endl;
          available_moves.push_back(move_obj(current_square, get_1d(tmp_coord), get_1d(target_coord), M_EP));
        }
      }
    }
  }
  //generate promotion
  // NOTE unfinished promotion move is made during the begining of the function
  // I need to look for them and elaborate/change them into promotion move
  //make a vector copy, erase the old vector, iterate through copy, for none promoting moves, simply add to vector, for
  // promoting moves, add the elaborated moves into the vector
  //lastly update the vector
  short promotion_row = side_to_move == 'b'? 0:7 ;
  //black pawn promote at bottom of the board, white pawn ... top of the board
  vector<move_obj> available_moves_copy = available_moves;
  available_moves.clear();
  for(move_obj move : available_moves_copy)
  {
    if(move.piece.type() == PAWN && get_row(move.to_square) == promotion_row)
    {//if this should have been a promotion move
      for(unsigned char type : {KNIGHT,BISHOP,ROOK,QUEEN})
      {
        move_obj new_move(move);
        new_move.promote_to.data = (moving_side_color | type);
        available_moves.push_back(new_move);
      }
    }
    else
    {
      available_moves.push_back(move); // put it back untouched
    }
  }
  //now available_moves is updated with correct promotion

  //generate castling
  //king cannot be attacked while castling
  // King cannot move through attacked square while castling
  //first check flag, then check the squares on castling path to see if it's attacked;

  // BLACK_KING_CASTLING_FLAG
  // BLACK_QUEEN_CASTLING_FLAG
  // WHITE_KING_CASTLING_FLAG
  // WHITE_QUEEN_CASTLING_FLAG

  //For each flag, Connditions to check
  // a: king cannot be attacked
  // b: path empty
  // c: path not attacked
  if(side_to_move == 'b')
  {
    if((board.castling_availability & BLACK_QUEEN_CASTLING_FLAG) == BLACK_QUEEN_CASTLING_FLAG)
    {
      bool a = !square_is_attacked(board,{7,4}, 'w');
      bool b = board.square[get_1d({7,1})].is_empty()
            && board.square[get_1d({7,2})].is_empty()
            && board.square[get_1d({7,3})].is_empty();
      bool c = !square_is_attacked(board,{7,2}, 'w')
           && !square_is_attacked(board,{7,3}, 'w');
      if(a && b && c)
      {
        move_obj tmp_move;
        tmp_move.piece.assign(BLACK|KING);
        tmp_move.from_square = tmp_move.to_square = 0;
        tmp_move.type = 4;//black queen side castling
        available_moves.push_back(tmp_move);
      }
    }
    if((board.castling_availability & BLACK_KING_CASTLING_FLAG) == BLACK_KING_CASTLING_FLAG)
    {
      bool a = !square_is_attacked(board,{7,4}, 'w');
      bool b = board.square[get_1d({7,5})].is_empty()
            && board.square[get_1d({7,6})].is_empty();
      bool c = !square_is_attacked(board,{7,5}, 'w')
           && !square_is_attacked(board,{7,6}, 'w');
      if(a && b && c)
      {
        move_obj tmp_move;
        tmp_move.piece.assign(BLACK|KING);
        tmp_move.from_square = tmp_move.to_square = 0;
        tmp_move.type = 3;//black king side castling
        available_moves.push_back(tmp_move);
      }
    }
  }
  else//white moves
  {
    if((board.castling_availability & WHITE_QUEEN_CASTLING_FLAG) == WHITE_QUEEN_CASTLING_FLAG)
    {
      bool a = !square_is_attacked(board,{0,4}, 'b');
      bool b = board.square[get_1d({0,1})].is_empty()
            && board.square[get_1d({0,2})].is_empty()
            && board.square[get_1d({0,3})].is_empty();
      bool c = !square_is_attacked(board,{0,2}, 'b')
           && !square_is_attacked(board,{0,3}, 'b');
      if(a && b && c)
      {
        move_obj tmp_move;
        tmp_move.piece.assign(WHITE|KING);
        tmp_move.from_square = tmp_move.to_square = 0;
        tmp_move.type = 6;//white queen side castling
        available_moves.push_back(tmp_move);
      }
    }
    if((board.castling_availability & WHITE_KING_CASTLING_FLAG) == WHITE_KING_CASTLING_FLAG)
    {
      bool a = !square_is_attacked(board,{0,4}, 'b');
      bool b = board.square[get_1d({0,5})].is_empty()
            && board.square[get_1d({0,6})].is_empty();
      bool c = !square_is_attacked(board,{0,5}, 'b')
           && !square_is_attacked(board,{0,6}, 'b');
      if(a && b && c)
      {
        move_obj tmp_move;
        tmp_move.piece.assign(WHITE|KING);
        tmp_move.from_square = tmp_move.to_square = 0;
        tmp_move.type = 5; //white king side castling
        available_moves.push_back(tmp_move);
      }
    }
  }


  // lastly, remove moves that will jeopardize my own king,
  vector<move_obj> moves_new;
  for(move_obj move : available_moves)
  {
    Chess_Board what_if(board);
    what_if.update(move);//what if I do this move
    //get new king location
    unsigned char tmp_king_location = side_to_move == 'b'? what_if.king_location[0]:what_if.king_location[1];
    if(!square_is_attacked(what_if, get_2d(tmp_king_location), other_side))
    {
      moves_new.push_back(move);
    }
  }
  // reassign available_moves
  available_moves = moves_new;

}
