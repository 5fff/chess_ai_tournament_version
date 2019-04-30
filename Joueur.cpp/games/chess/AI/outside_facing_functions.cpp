#include "outside_facing_functions.h"
#include <ctime>
#include <cstdlib>
string get_random_move(string fen_string)
{
  srand(time(NULL));
  Chess_Board b;
  vector<move_obj> available_moves;
  b.load_fen(fen_string);
  generate_moves(available_moves,b);

  short random_index = rand()%(available_moves.size());
  cout<<"Available Moves Are:"<<endl;
  for(auto&& move : available_moves)
  {
    cout<<translate_to_san(move)<<" ";
  }
  cout<<endl;
  string move_to_return = translate_to_san(available_moves[random_index]);
  cout<<"Selecting Move, not including parentheses("<<move_to_return<<")"<<endl;
  return move_to_return;
}
