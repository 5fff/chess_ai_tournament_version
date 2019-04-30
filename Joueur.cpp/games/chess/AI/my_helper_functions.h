#ifndef MHF_HPP
#define MHF_HPP

#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <chrono>
#include "common_var.h"
#include "my_chess_board.h"

using namespace std;

#define ROW 0
#define COL 1


string get_san_coord(short index);

// linear representation to 2d conversion
short get_row(short index);
short get_col(short index);

pair<short,short> get_2d(short index);

// 2d coordinate to linear (1d)
short get_1d(short row, short col);
short get_1d(pair<short,short> coord);

bool is_bounded(pair<short,short> coord);
bool is_bounded(short row, short col);

pair<short,short> operator+ (pair<short,short>& lhs, pair<short,short>& rhs);
pair<short,short> operator- (pair<short,short>& lhs, pair<short,short>& rhs);

vector<std::string> string_split(std::string str_to_split, char delimeter);

string prettyFEN(string fen, string us);

//this function will derive the move from old and new fen strings combined with san string
//it is quicker and easier than parsing the san string with the previous state.
move_obj derive_move_obj(Chess_Board board_0, Chess_Board board_1, string san_str);

//test quiesence status, return true if is non-quiesence,  false otherwise


void print_raw_move(move_obj& move);

bool nqs_test(Chess_Board& board);


#endif
