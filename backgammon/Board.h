#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <time.h>
#include <unordered_map>
#include <array>

// todo: AI.
const int PIPS = 15;

class Board {

public:
    Board();
    void        reset();
    void        print_board() const;
    bool        test_space_empty(int space) const;
    std::string color_on_space(int space) const;
    int         num_pips_on_space(int space) const;
    void        remove_one_from_space(int space);
    void        add_one_to_space(int space, std::string color);
    bool        hitable(int space) const;
    void        print_space(int space) const;
    void        add_knocked(std::string color);
    void        remove_knocked(std::string color);
    int         get_knocked(std::string color) const;
    void        add_one_off_board(std::string color);
    bool        gameover() const { return white_win() or black_win(); };

    std::string get_winner() const { return white_win() ? "white" : "black"; };
    bool white_win() const { return (players.at("white").num_off == PIPS); };
    bool black_win() const { return (players.at("black").num_off == PIPS); };
    void set_score(int w, int b) {
        players["white"].score = w;
        players["black"].score = b;
    }
    int any_black_in_white_home() const;
    int any_white_in_black_home() const;
    int num_off(std::string color) const { return players.at(color).num_off; };
    int num_knocked(std::string color) const {
        return players.at(color).num_knocked;
    };

    int  double_amount() const { return double_cube; }
    void set_double(int d) { double_cube = d; };

    void decline_double(std::string color) {
        players[othercolor(color)].num_off = PIPS;
    };

private:
    std::array<std::array<std::string, 15>, 24> board;
    std::pair<int, int>                         dice;
    int                                         double_cube;
    std::unordered_map<std::string, Player>     players;

    std::string othercolor(std::string color) {
        return color == "white" ? "black" : "white";
    }
};
#endif
