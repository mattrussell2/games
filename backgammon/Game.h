#ifndef GAME_H
#define GAME_H

#include "Board.h"

typedef std::pair<int, int> Move;

class Game {
public:
    Game() {
        double_turn     = "empty";
        first_move      = true;
        white_score     = 0;
        black_score     = 0;
        double_declined = false;
    };

    void run_game();
    void determine_first_turn();

    bool gameover() { return b.gameover(); }
    bool check_move(int start, int end, bool print = false);
    bool determine_valid_moves(bool debug = false);
    Move get_move();
    
    void make_move(int first_pos, int second_pos);
    void take_turn();
    bool test_in_zone();
    bool check_zone_move(int start, int end, bool print = false);
    bool validate_input(const std::string& strin) const;
    bool double_cube();

    std::string report_score() const {
        return "the score is --- white: " + std::to_string(white_score) +
               " black: " + std::to_string(black_score);
    }

    std::string report_winner() const {
        if (white_score > black_score) return "white wins!";
        if (black_score > white_score) return "black wins!";
        return "it's a tie!!!";
    }

    std::pair<int, int> roll_dice();

private:
    Board b;

    std::string turn;

    bool        first_move;
    bool        double_declined;
    std::string double_turn;

    int white_score;
    int black_score;

    std::pair<int, int> dice{-1, -1};
    Move                move_taken{false, false};

    std::string othercolor(std::string color) {
        return color == "white" ? "black" : "white";
    }

    void end_game(std::string winner);
};

#endif
