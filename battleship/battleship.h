// battleship.h
// by matt russell
// 2019

#include "ship.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::vector<std::string>> board;
typedef std::vector<std::vector<int>>         pboard;

class battleship {
public:
    battleship();

    void print_game() const;
    void start_game();
    void pc_test();
    void run_game();

private:
    void init_board(board& b);
    void init_pboard(pboard& pb);

    void   find_best_guess(board& b, pboard& pb, pos& p);
    bool   is_perfect_loc(const board& b, pos& p) const;
    double dist_from_center(pos p) const;
    bool   is_valid_loc(const board& b, pos p) const;
    bool   blood_in_the_water(const board& b, pos& p) const;
    bool   search_for_hit_location(const board& b, pos& p) const;

    void calc_probabilities(board& b, pboard& pb);
    void find_medium_good_spot(const board& b, pos& p) const;

    void easy_guess(const board& b, pos& p) const;

    void update_probabilities(board& b, pboard& pb, pos p, int len, bool d);
    bool check_nearby_hit(const board& b, int len, pos p, int d) const;

    void get_start_coord(const board& b, int len, pos& p, bool& d) const;
    void generate_ships(board& b, vector<ship>& ships, bool hu_pc);
    void find_valid_start_loc(const board& b, int len, pos& p, bool& d) const;
    bool check_clear_area(const board& b, int len, pos p, int d) const;
    void place_ship(board& b, int len, pos p, bool d, string s);

    void register_hit(board& hit_board, board& fire_board, pboard& fire_pboard,
                      pos p);
    void register_miss(board& missed_board, board& f_board, pboard& f_pboard,
                       pos p);

    bool test_for_hit(const board& b, pos p) const;

    bool game_ready(const board& human_board) const;
    bool gameover() const;

    // these two functions make the guesses and call register hits/miss fns.
    bool   hu_make_guess(pos& p);
    string pc_make_guess();

    void hu_process_guess(const pos& p);

    // first board is the TRY board; add letters to it if a ship is sunk
    void add_hit_to_ships(board& b, vector<ship>& ships, pboard& pb, pos p,
                          bool disp, bool isPCTurn);

    // searches through the ships to find which one is sunk
    void check_sunken_ships(const board& hit_board, vector<ship>& ships) const;

    void print_board(const board& b) const;
    void print_two_boards(const board& b, const board& b2) const;
    void print_user_game_data() const;
    void print_full_game_data(string outstr) const;

    board pc_own_board;
    board pc_try_board;
    board hu_own_board;
    board hu_try_board;

    pboard hu_pboard;
    pboard pc_pboard;

    int         ship_len_lookup[5]   = {5, 4, 3, 3, 2};
    std::string ship_class_lookup[5] = {
        "\u001b[33;1mA\u001b[0m", "\u001b[36;1mB\u001b[0m",
        "\u001b[37;1mC\u001b[0m", "\u001b[35;1mS\u001b[0m",
        "\u001b[32;1mD\u001b[0m"};
    std::string  ship_name_lookup[5] = {"Aircraft Carrier", "Battleship",
                                       "Crusier", "Submarine", "Destroyer"};
    vector<ship> pc_ships;
    vector<ship> hu_ships;

    string difficulty;

    vector<string> pc_guesses;
    vector<string> hu_guesses;

    // 'board' strings
    const std::string B_EMPTY_OCEAN = "\033[1;34m*\033[0m";
    const std::string B_MISSED_SHOT = "\033[1;37m_\033[0m";
    const std::string B_HIT_SHIP    = "\033[1;31mX\033[0m";

    // 'info' strings
    const std::string I_MISSED_SHOT = "\033[1;37mHaha! You Missed!\033[0m";
    const std::string I_HIT_SHOT    = "\033[1;31mDrats! You Hit!\033[0m";
};