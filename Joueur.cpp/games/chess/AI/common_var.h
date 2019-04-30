#ifndef COMMON_H
#define COMMON_H
#include <map>
#include <array>
#include <string>

using namespace std;

extern const unsigned char TYPE_MASK,PAWN,KNIGHT,BISHOP,ROOK,QUEEN,KING,IS_PIECE,COLOR_MASK,BLACK,WHITE,EMPTY;

extern map<char, unsigned char> castling_flags;
extern map<char, unsigned char> char2code;
extern map<unsigned char, char> code2char;
extern map<unsigned char, char> piece2value;
extern map<char, array<short,2>> char2aux;
extern map<string, unsigned char> SAN_1D;
extern const string SAN_TABLE[];

extern const unsigned char M_REG,M_CAP,M_EP,M_BKC,M_BQC,M_WKC,M_WQC,M_JJ;
extern pair<short,short> DIRECTION[];
extern pair<short,short> HORSE_VECTOR[];
extern pair<short,short> V_SHAPE[];
extern pair<short,short> A_SHAPE[];


extern pair<short,short> UP,UP2,DOWN,DOWN2,LEFT,RIGHT;

extern const unsigned char BLACK_KING_CASTLING_FLAG  ;
extern const unsigned char BLACK_QUEEN_CASTLING_FLAG ;
extern const unsigned char WHITE_KING_CASTLING_FLAG  ;
extern const unsigned char WHITE_QUEEN_CASTLING_FLAG ;
extern const unsigned char NOTHING                   ;
#endif
