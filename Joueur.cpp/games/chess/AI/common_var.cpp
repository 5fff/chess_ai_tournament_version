#include "common_var.h"

using namespace std;

/*
const unsigned char  TYPE_MASK   =  0b00'111'000;

const unsigned char  PAWN        =  0b00'000'000;
const unsigned char  KNIGHT      =  0b00'001'000;
const unsigned char  BISHOP      =  0b00'010'000;
const unsigned char  ROOK        =  0b00'011'000;
const unsigned char  QUEEN       =  0b00'100'000;
const unsigned char  KING        =  0b00'101'000;

const unsigned char  IS_PIECE    =  0b1'0000000;
const unsigned char  COLOR_MASK  =  0b11'000000;//color mask to get WHITE/BLACK/EMPTY
const unsigned char  BLACK       =  0b10'000000;
const unsigned char  WHITE       =  0b11'000000;
const unsigned char  EMPTY       =  0b00'000000;
// const unsigned char  AUX_FLAG    =  0b00000'1'00;

const unsigned char BLACK_KING_CASTLING_FLAG  = 0b10000000;
const unsigned char BLACK_QUEEN_CASTLING_FLAG = 0b01000000;
const unsigned char WHITE_KING_CASTLING_FLAG  = 0b00100000;
const unsigned char WHITE_QUEEN_CASTLING_FLAG = 0b00010000;
const unsigned char NOTHING                   = 0b00000000;

*/

const unsigned char  TYPE_MASK   =  0b00111000;

const unsigned char  PAWN        =  0b00000000;
const unsigned char  KNIGHT      =  0b00001000;
const unsigned char  BISHOP      =  0b00010000;
const unsigned char  ROOK        =  0b00011000;
const unsigned char  QUEEN       =  0b00100000;
const unsigned char  KING        =  0b00101000;

const unsigned char  IS_PIECE    =  0b10000000;
const unsigned char  COLOR_MASK  =  0b11000000;//color mask to get WHITE/BLACK/EMPTY
const unsigned char  BLACK       =  0b10000000;
const unsigned char  WHITE       =  0b11000000;
const unsigned char  EMPTY       =  0b00000000;
// const unsigned char  AUX_FLAG    =  0b00000'1'00;

const unsigned char BLACK_KING_CASTLING_FLAG  = 0b10000000;
const unsigned char BLACK_QUEEN_CASTLING_FLAG = 0b01000000;
const unsigned char WHITE_KING_CASTLING_FLAG  = 0b00100000;
const unsigned char WHITE_QUEEN_CASTLING_FLAG = 0b00010000;
const unsigned char NOTHING                   = 0b00000000;
map<char, unsigned char> castling_flags=
{
  {'k',BLACK_KING_CASTLING_FLAG},
  {'q',BLACK_QUEEN_CASTLING_FLAG},
  {'K',WHITE_KING_CASTLING_FLAG},
  {'Q',WHITE_QUEEN_CASTLING_FLAG},
  {'Q',NOTHING}
};

// type: 0 = Regular Move
// type: 1 = Capture Move
// type: 2 = En Passant Capture
// type: 3 = k black king side castle
// type: 4 = q black queen side castle
// type: 5 = K white king side castle
// type: 6 = Q white queen side castle

const unsigned char M_REG = 0;
const unsigned char M_CAP = 1;
const unsigned char M_EP  = 2;
const unsigned char M_BKC = 3;
const unsigned char M_BQC = 4;
const unsigned char M_WKC = 5;
const unsigned char M_WQC = 6;
const unsigned char M_JJ = 7;


map<char, unsigned char> char2code=
{
  {'p',BLACK|PAWN},
  {'n',BLACK|KNIGHT},
  {'b',BLACK|BISHOP},
  {'r',BLACK|ROOK},
  {'q',BLACK|QUEEN},
  {'k',BLACK|KING},
  {'P',WHITE|PAWN},
  {'N',WHITE|KNIGHT},
  {'B',WHITE|BISHOP},
  {'R',WHITE|ROOK},
  {'Q',WHITE|QUEEN},
  {'K',WHITE|KING}
};

map<unsigned char, char> code2char=
{
  {BLACK|PAWN,'p'},
  {BLACK|KNIGHT,'n'},
  {BLACK|BISHOP,'b'},
  {BLACK|ROOK,'r'},
  {BLACK|QUEEN,'q'},
  {BLACK|KING,'k'},
  {WHITE|PAWN,'P'},
  {WHITE|KNIGHT,'N'},
  {WHITE|BISHOP,'B'},
  {WHITE|ROOK,'R'},
  {WHITE|QUEEN,'Q'},
  {WHITE|KING,'K'},
  {EMPTY, '.'}
};


// 1 = a pawn
// 3 = a Knight or Bishop
// 5 = a Rook
// 9 = a Queen
//white positive, black negative
map<unsigned char, char> piece2value=
{
  {BLACK|PAWN, -1},
  {BLACK|KNIGHT, -3},
  {BLACK|BISHOP, -3},
  {BLACK|ROOK, -5},
  {BLACK|QUEEN, -9},
  {BLACK|KING, 0},//0 value so it does not affect the sum of material value, ignores king
  {WHITE|PAWN, 1},
  {WHITE|KNIGHT, 3},
  {WHITE|BISHOP, 3},
  {WHITE|ROOK, 5},
  {WHITE|QUEEN, 9},
  {WHITE|KING, 0},
  {EMPTY, 0} //0 value does not affect sum, ignored
};


map<char, array<short,2>> char2aux =
{
  {'q',{0,4}},
  {'k',{4,7}},
  {'Q',{56,60}},
  {'K',{60,63}}
};


//NOTE Bottom left of the board is 0,0

const string SAN_TABLE[] =
{
"a1","b1","c1","d1","e1","f1","g1","h1",
"a2","b2","c2","d2","e2","f2","g2","h2",
"a3","b3","c3","d3","e3","f3","g3","h3",
"a4","b4","c4","d4","e4","f4","g4","h4",
"a5","b5","c5","d5","e5","f5","g5","h5",
"a6","b6","c6","d6","e6","f6","g6","h6",
"a7","b7","c7","d7","e7","f7","g7","h7",
"a8","b8","c8","d8","e8","f8","g8","h8"
};

map<string, unsigned char> SAN_1D =
{
{"a1", 0 },	{"b1", 1 },	{"c1", 2 },	{"d1", 3 },	{"e1", 4 },	{"f1", 5 },	{"g1", 6 },	{"h1", 7 },
{"a2", 8 },	{"b2", 9 },	{"c2", 10 },	{"d2", 11 },	{"e2", 12 },	{"f2", 13 },	{"g2", 14 },	{"h2", 15 },
{"a3", 16 },	{"b3", 17 },	{"c3", 18 },	{"d3", 19 },	{"e3", 20 },	{"f3", 21 },	{"g3", 22 },	{"h3", 23 },
{"a4", 24 },	{"b4", 25 },	{"c4", 26 },	{"d4", 27 },	{"e4", 28 },	{"f4", 29 },	{"g4", 30 },	{"h4", 31 },
{"a5", 32 },	{"b5", 33 },	{"c5", 34 },	{"d5", 35 },	{"e5", 36 },	{"f5", 37 },	{"g5", 38 },	{"h5", 39 },
{"a6", 40 },	{"b6", 41 },	{"c6", 42 },	{"d6", 43 },	{"e6", 44 },	{"f6", 45 },	{"g6", 46 },	{"h6", 47 },
{"a7", 48 },	{"b7", 49 },	{"c7", 50 },	{"d7", 51 },	{"e7", 52 },	{"f7", 53 },	{"g7", 54 },	{"h7", 55 },
{"a8", 56 },	{"b8", 57 },	{"c8", 58 },	{"d8", 59 },	{"e8", 60 },	{"f8", 61 },	{"g8", 62 },	{"h8", 63 }
};


//  3      2      1
//      \  ^  /
//  4   <  X  >   0
//      /  v  \
//  5      6      7

// 3 2 1
// 4 X 0
// 5 6 7

//row, col
pair<short,short> DIRECTION[] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

pair<short,short> HORSE_VECTOR[] = {{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};
pair<short,short> V_SHAPE[] = {DIRECTION[1],DIRECTION[3]};
pair<short,short> A_SHAPE[] = {DIRECTION[5],DIRECTION[7]};

pair<short,short> UP = {1,0};
pair<short,short> UP2 = {2,0};
pair<short,short> DOWN = {-1,0};
pair<short,short> DOWN2 = {-2,0};
pair<short,short> LEFT = {0,-1};
pair<short,short> RIGHT = {0,1};
