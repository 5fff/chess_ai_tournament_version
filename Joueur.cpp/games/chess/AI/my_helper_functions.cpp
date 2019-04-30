// #include "common_var.h"
#include "my_helper_functions.h"
#include "common_var.h"
#include "move_generation_functions.h"
// #include "my_chess_board.h"
//returs a san notation according to a index 0 to 63
//e.g.


short get_row(short index){return index/8;}
short get_col(short index){return index%8;}

pair<short,short> get_2d(short index){return {(index/8), (index%8)};}

// 2d coordinate to linear (1d)
short get_1d(short row, short col){return row*8 + col;}
short get_1d(pair<short,short> coord)
{
  // if(!is_bounded(coord))
  // {
  //   cout<<"Get_1d out of bound! row="<<get<ROW>(coord)<<" col="<<get<COL>(coord)<<endl;
  //   throw;
  // }
  return get<ROW>(coord)*8 + get<COL>(coord);
}

bool is_bounded(pair<short,short> coord){return (get<ROW>(coord) >= 0 && get<ROW>(coord) < 8 && get<COL>(coord) >= 0 && get<COL>(coord) < 8);}
bool is_bounded(short row, short col){return (row >= 0 && row < 8 && col >= 0 && col < 8);}


string get_san_coord(short index)
{
  return SAN_TABLE[index];
}

pair<short,short> operator+ (pair<short,short>& lhs, pair<short,short>& rhs)
{
  return {get<0>(lhs)+get<0>(rhs), get<1>(lhs)+get<1>(rhs)};
}

pair<short,short> operator- (pair<short,short>& lhs, pair<short,short>& rhs)
{
  return {get<0>(lhs)-get<0>(rhs), get<1>(lhs)-get<1>(rhs)};
}

/// <summary> This is copied from ai.cpp, I moved the function to here
/// std::string split implementation by using delimiter as a character.
/// </summary>
vector<std::string> string_split(std::string str_to_split, char delimeter)
{
    std::stringstream ss(str_to_split);
    std::string item;
    std::vector<std::string> split_strings;
    while (std::getline(ss, item, delimeter))
    {
        split_strings.push_back(item);
    }

    return split_strings;
}

string prettyFEN(string fen, std::string us)
{
    // split the FEN string up to help parse it
    auto split = string_split(fen, ' ');
    auto first = split[0]; // the first part is always the board locations

    auto sideToMove = split[1].at(0); // always the second part for side to move
    auto usOrThem = sideToMove == us.at(0) ? "us" : "them";

    auto fullmove = split[5]; // always the sixth part for the full move

    auto lines = string_split(first, '/');
    std::stringstream strings;
    strings << "Move: " << fullmove << "\n"
            << "Side to move: " << sideToMove << " (" << usOrThem << ")\n"
            << "   +-----------------+";

    int i = -1;
    for (auto line : lines)
    {
        i++;
        strings << "\n " << (8 - i) << " |";
        for (char &character : line)
        {
            int asInt = (int)character - 48; // test for char '1' -> int 1
            if (asInt > -1 && asInt < 9)
            {
                // blank tiles n times, so repeat the . that many times
                for (int j = 0; j < asInt; j++)
                {
                    strings << " .";
                }
            }
            else
            {
                // it was a regular character for a piece
                strings << ' ' << character;
            }
        }
        strings << " |";
    }
    strings << "\n   +-----------------+\n     a b c d e f g h\n";

    return strings.str();
}

//this function will derive the move from old and new Chess_Board object(converted from fen strings) combined with san string
//it is quicker and easier than parsing the san string with the previous state.
move_obj derive_move_obj(Chess_Board board_0, Chess_Board board_1, string san_str)
{
  // cout<<"board_0"<<endl;
  // board_0.print_board();
  // cout<<"board_1"<<endl;
  // board_1.print_board();

  //fen_0 is board before move, fen_1 is board after move
  //first check if it is special move, namely O-O-O or O-O (both color)
  //###########deal with castling moves below ##########
  //if the san string start with '0' or 'O' it must be castling move
  if(san_str[0] == '0' || san_str[0] == 'O')
  {
    Chess_Piece tmp_piece;
    tmp_piece.assign(EMPTY); //piece type doesn't matter, only the castling flag is needed
    move_obj tmp_move_obj(tmp_piece, 0,0, M_BKC);
    // it doesn't matter what type of castling move it is , as long as it is a castling move
    //it will trigger the STFR tracker to clear, see three_fold_tracker implementation for
    // explanation on this.
    return tmp_move_obj;
  }
  //####################################################
  //for non castling moves, pawn moves
  //as long as it is a pawn move, it will trigger the three fold repetition
  // discard the move history hence the move location is never used, so this function
  // simply mark a move as a pawn move without any location information
  // pawn moves include regular pawn moves, double jump moves, pawn captures,
  // en passant captures, promotion
  //to detect all of the above, simply detect the missing type letter (e.g. K,Q,N etc.)
  //STANDARD: if its a pawn move, the move_obj's Piece will be a plain PAWN flag without any other information
  if(san_str[0] >= 'a' && san_str[0] <= 'h')
  {
    //then its a pawn move
    Chess_Piece tmp_piece;
    tmp_piece.assign(PAWN); //sufficient to mark it as a pawn move
    move_obj tmp_move_obj(tmp_piece, 0,0,0); // 0,0,0 can be any value, it is being ignored as soon as PAWN move detected
    return tmp_move_obj;
  }
  //for non castling or pawn moves
  //compare two board states, and figure out what piece is moved and wether a capture happend

  Chess_Piece tmp_piece;
  move_obj tmp_move_obj;
  //first find starting cell
  for(short i = 0; i < 64; i++)
  {
    // if the cell changed form some piece to empty then it moved away
    if(board_0.square[i].not_empty() && board_1.square[i].is_empty())
    {
      //found what piece is moving
      tmp_piece.assign(board_0.square[i]);
      tmp_move_obj.piece = tmp_piece;
      tmp_move_obj.from_square = i;
      break;
    }
  }
  //then find destination cell and capture or not
  for(short i = 0; i < 64; i++)
  {
    // if the cell changed form some piece to empty then it moved away
    if(board_0.square[i].is_empty() && board_1.square[i].not_empty())
    {
      //find where that piece is moving to, and it's a regular move(not capture)
      tmp_move_obj.type = M_REG;
      tmp_move_obj.to_square = i;
      break;
    }
    if(board_0.square[i].not_empty() && board_1.square[i].not_empty() && (board_0.square[i].data != board_1.square[i].data))
    {
      //it's a capture move
      tmp_move_obj.type = M_CAP;
      tmp_move_obj.to_square = i;
      break;
    }
  }
  return tmp_move_obj;



}


void print_raw_move(move_obj& move)
{
  cout<<"Move Obj Dump:"<<(short)move.piece.data<<" "<<(short)move.promote_to.data<<" "<<(short)move.from_square<<" "<<(short)move.to_square<<" "<<(short)move.type<<endl;
}

bool nqs_test(Chess_Board& board)
{
  //current test logic:
  // return true if any king is in check
  //side to move     king location       other_side
  //  b                 [0]                 w
  //  w                 [1]                 b
  if( square_is_attacked(board, get_2d(board.king_location[0]), 'w') ||
      square_is_attacked(board, get_2d(board.king_location[1]), 'b'))
  {
    return true;
  }
  return false;

}
