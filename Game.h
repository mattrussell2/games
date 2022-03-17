#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game {
public:
    Game() {
        double_turn     = "empty";
        first_move      = true;
        white_score     = 0;
        black_score     = 0;
        double_declined = false;
    };
    ~Game() { delete[] dice; };
    void run_game();
    void welcome_message();
    bool check_gameover() { return b.check_gameover(); }
    bool check_move(int start, int end, bool print = false);
    bool determine_valid_moves(bool debug = false);
    bool get_move(int& first_pos, int& second_pos);
    void make_move(int first_pos, int second_pos);
    void take_turn();
    void determine_first_turn();
    bool test_in_zone();
    bool check_zone_move(int start, int end, bool print = false);
    bool validate_input(const std::string& strin) const;
    bool double_cube();
    int  white_score;
    int  black_score;

private:
    // int         first_pos, second_pos;
    // std::string strin1, strin2;
    bool        first_move;
    std::string turn;
    bool        double_declined;

    // dynamically allocated
    Board b;
    int*  dice;
    bool  curr_taken[2]{false, false};

    std::string double_turn;
    void        end_game(std::string winner);
    //  bool black_in_zone, white_in_zone;
};

#endif
