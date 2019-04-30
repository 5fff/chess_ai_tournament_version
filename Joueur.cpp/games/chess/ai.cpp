// AI
// This is where you build your AI

#include "ai.hpp"
#include <vector>
#include <sstream>
#include <chrono>
#include <string>

#define DEFAULT_QS_DEPTH 5
#define MIN_DEPTH 3
#define RESERVE_MOVES 60

namespace cpp_client
{

namespace chess
{

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // REPLACE WITH YOUR TEAM NAME!
    return "ray_chess_ai";
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // This is a good place to initialize any variables
    cout<<"###########Game Started!!!!!!#########"<<endl;
    cout<<this->game->fen<<endl;
    cout<<"my color is "<<this->player->color<<endl;
    cout<<"######################################"<<endl;
    enderchest.qs_depth_limit = DEFAULT_QS_DEPTH;
    try
    {
      enderchest.qs_depth_limit = stoi(this->get_setting("qs_depth"));
    }
    catch (const std::invalid_argument& ia)
    {
      cout<<"invalid input! Using default"<<endl;
    }
    cout<<"qs_limit = "<<enderchest.qs_depth_limit<<endl;
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{

}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string &reason)
{
    // You can do any cleanup of your AI here.  The program ends when this function returns.
}

/// <summary>
/// This is called every time it is this AI.player's turn to make a move.
/// </summary>
/// <returns>A string in Standard Algebriac Notation (SAN) for the move you want to make. If the move is invalid or not properly formatted you will lose the game.</returns>
std::string AI::make_move()
{
    /*******************************************************************************************************
     * IMPORTANT SERVER VARIABLES TO KNOW:
     *******************************************************************************************************
     *
     * * Game.History
     *      - The list of moves that have occurred in the game so far in SAN.
     *
     * * Game.Fen
     *      - The FEN string representing the current board state. Updated every turn
     *      - For more info about FEN strings: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
     *
     * * Player
     *      - Your player object.
     *      - Important properties:
     *          * Player.Color
     *              - Your client's color ("white" or "black")
     *          * Player.Opponent
     *              - Your opponent's player object.
     *
     * NOTE THAT ALL OBJECT INFORMATION IS CONTAINED IN Joueur.cpp/games/chess/
     * HOWEVER, DO NOT CHANGE ANY OF THESE FILES EXCEPT FOR ai.cpp, ai.hpp, AND ANY OTHER FILES YOU MAY ADD YOURSELF
     *
     *******************************************************************************************************
     * STANDARD ALGEBRAIC NOTATION
     *******************************************************************************************************
     *
     * * When returning your move in Standard Algebraic Notation (SAN), the way I recommend is to first
     *   indicate the piece type if the piece is not a pawn, then the starting square's file and rank, then
     *   an x if there is a capture, then the ending square's file and rank.
     *
     * * Pieces are indicated as R for Rook, N for Knight, B for Bishop, Q for Queen, and K for King. Pawns
     *   are not denoted by any letter.
     *
     * * For castling in SAN, king-side castling is indicated by "O-O", while queen-side castling is
     *   indicated by "O-O-O".
     *
     * * To promote a pawn when it reaches the other side of the board, add the type of piece to promote to
     *   at the end of the string.
     *
     * * All SAN is case sensitive.
     *
     * * Some examples:
     *      - Move a pawn from a2 to a4: a2a4
     *      - Capture a piece with a pawn from c6 to d5: c6xd5
     *      - Move a knight from b1 to c3: Nb1c3
     *      - Capture a piece with a queen from h3 to e6: Qh3xe6
     *      - King-side castle: O-O
     *
     * * For more info about SAN: https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
     *
     *******************************************************************************************************/
     cout<<"\n\n\n"<<endl;
     double time_remaining = this->player->time_remaining;
     cout<<"time_left:"<<time_remaining<<"ns"<<endl;
     //log board and get opponents move(if any)
     if(!(this->game->history.empty()))
     {//if not at begining of the game
       eternal_historian.update_before_move(this->game->fen, this->game->history.back());
     }
     else
     { //if no moves has been made, you make the first move
       // during the first start of the game this brach will ran , and the san string will be ignored
       eternal_historian.update_before_move(this->game->fen, "NULL");
     }


    std::cout << prettyFEN(this->game->fen, this->player->color);

    // cout<<this->game->fen<<endl;



    Minimax smart_monkey;//just to distinguish it from class name

    long time_limit = time_remaining/1000000/RESERVE_MOVES;//convert ns to ms
    cout<<"Using time limit: "<<time_limit<<endl;
    //calculate time spend in finding move

    smart_monkey.get_move(this->game->fen, eternal_historian.tracker, time_limit, enderchest.qs_depth_limit);

    // timing logic: assuming my following moves will always take about the same time, then calculate how many moves
    // I can still think after this move, if the estimated number of moves I can make is smaller than predefind limit,
    // then decrease depth limit to speedup the search process
    // to prevent irrational moves under limited time, there's a minimum depth limit,
    // my reasoning is that if both player has ran out of time, I have a better chance to "stay calm",
    // there's indeed a risk of timing out, but I have a better chance to make a smarter move and end the game before timeout(may not be correct)
    //after finding the move tell the persistent tracker,"eternal_historian", the move we made and the new board state
    eternal_historian.update_after_move(smart_monkey.get_next_board(), smart_monkey.best_move);
    return smart_monkey.selected_move_str;
}

// You can add additional methods here for your AI to call

} // namespace chess

} // namespace cpp_client
