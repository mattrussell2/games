/*
 * battleship.cpp
 * by matt russell
 * 2019 - release
 * 2022 - minor updates
 */

#include "battleship.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <time.h>

void battleship::start_game() {
    std::cout << R"(
___.              __     __   .__                   .__     .__         
\_ |__  _____   _/  |_ _/  |_ |  |    ____    ______|  |__  |__|______
 | __ \ \__  \  \   __\\   __\|  |  _/ __ \  /  ___/|  |  \ |  |\____ \
 | \_\ \ / __ \_ |  |   |  |  |  |__\  ___/  \___ \ |   Y  \|  ||  |_> >
 |___  /(____  / |__|   |__|  |____/ \___  >/____  >|___|  /|__||   __/  
     \/      \/                          \/      \/      \/     |__|     
	    )";
    std::cout << "\nWelcome to battleship!\nYou have the option to place"
              << " the ships on the board, or can have me do it for you.\n"
              << "Would you like to place the ships on the board yourself"
              << " (y/n)? ";

    std::string s;
    getline(cin, s);
    while (s != "y" && s != "n" && s != "Y" && s != "N") {
        std::cout << "please enter y/n: ";
        getline(cin, s);
    }
    std::cout << "\n";

    srand(time(NULL));
    if (s == "y" || s == "Y") {
        generate_ships(hu_own_board, hu_ships, true);
    } else {
        generate_ships(hu_own_board, hu_ships, false);
    }
    generate_ships(pc_own_board, pc_ships, false);

    std::cout << "Okay! How difficult would you like me?" << std::endl;
    std::cout << "(0 = easy, 1 = medium, 2 = hard): ";
    getline(cin, s);
    while (s != "0" && s != "1" && s != "2") {
        std::cout << "Enter 0 or 1 or 2: ";
        getline(cin, s);
    }
    if (s == "0") difficulty = "easy";
    if (s == "1") difficulty = "medium";
    if (s == "2") difficulty = "hard";
    std::cout << std::endl;
    print_user_game_data();
    std::cout << std::endl;
}

battleship::battleship() {
    init_board(pc_try_board);
    init_board(hu_try_board);
    init_board(pc_own_board);
    init_board(hu_own_board);
    init_pboard(pc_pboard);
    init_pboard(hu_pboard);
}

// initialize a 10x10 probability board
void battleship::init_pboard(pboard& pb) {
    vector<int> a;
    for (int i = 0; i < 10; i++) {
        pb.push_back(a);
        for (int j = 0; j < 10; j++) {
            pb[i].push_back(0);
        }
    }
}

// initialize a board of ships or attempt board
void battleship::init_board(board& b) {
    vector<string> v;
    for (int i = 0; i < 10; i++) {
        b.push_back(v);
        for (int j = 0; j < 10; j++) {
            b[i].push_back(B_EMPTY_OCEAN);
        }
    }
}

// given a board and a location, test if there's a hit ship there
bool battleship::test_for_hit(const board& b, pos p) const {
    return (b[p.y][p.x] != B_EMPTY_OCEAN && b[p.y][p.x] != B_MISSED_SHOT &&
            b[p.y][p.x] != B_HIT_SHIP);
}

// test for end of game (if there are 17 hits, gg)
bool battleship::gameover() const {
    int pc_count  = 0;
    int usr_count = 0;
    pos p;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            p = pos(i, j);
            if (test_for_hit(pc_try_board, p)) pc_count++;
            if (test_for_hit(hu_try_board, p)) usr_count++;
        }
    }
    if (pc_count == 17) {
        print_full_game_data("I WIN. BWAHAHAHAHAA!!!!");
        return true;
    }
    if (usr_count == 17) {
        print_full_game_data("YOU WIN. NOOOOOOOOOOOO!!!");
        return true;
    }
    return false;
}

// adds the hit to the vector of ships; if a ship is sunk, display info
void battleship::add_hit_to_ships(board& b, vector<ship>& ships, pboard& pb,
                                  pos p, bool disp, bool isPCTurn) {
    int q = (int)ships.size();
    for (int i = 0; i < q; i++) {
        if (ships[i].within_ship(p)) {
            ships[i].add_hit_to_ship(p);
            if (ships[i].is_sunk()) {
                ship s = ships[i];
                if (disp) {
                    if (isPCTurn) {
                        std::cout << "HAHA! I SUNK YER SHIP: ";
                        string c = s.get_char();
                        if (c == "\u001b[33;1mA\u001b[0m")
                            std::cout << "OHO! An Aircraft Carrier!"
                                      << std::endl;
                        if (c == "\u001b[36;1mB\u001b[0m")
                            std::cout << "TEE HEE HEE! A Battleship!"
                                      << std::endl;
                        if (c == "\u001b[37;1mC\u001b[0m")
                            std::cout << "JOY! A Cruiser!" << std::endl;
                        if (c == "\u001b[35;1mS\u001b[0m")
                            std::cout << "BREAK OUT THE RUM! A Submarine!"
                                      << std::endl;
                        if (c == "\u001b[32;1mD\u001b[0m")
                            std::cout << "GOT EM! A Destroyer" << std::endl;
                    } else {
                        std::cout << "ARGH! YOUVE SUNK ME SHIP: ";
                        string c = s.get_char();
                        if (c == "\u001b[33;1mA\u001b[0m")
                            std::cout << "DRATS! An Aircraft Carrier!"
                                      << std::endl;
                        if (c == "\u001b[36;1mB\u001b[0m")
                            std::cout << "OH NO! A Battleship!" << std::endl;
                        if (c == "\u001b[37;1mC\u001b[0m")
                            std::cout << "EGADS! A Cruiser!" << std::endl;
                        if (c == "\u001b[35;1mS\u001b[0m")
                            std::cout << "GOODNESS! A Submarine!" << std::endl;
                        if (c == "\u001b[32;1mD\u001b[0m")
                            std::cout << "RATS! A Destroyer" << std::endl;
                    }
                }
                string sunk_string =
                    "\033[1;31m" + s.get_name().substr(0, 1) + "\033[0m";
                // replace the ship on the try board with the char of the ship
                place_ship(b, s.get_length(), s.get_start_pos(), s.get_dir(),
                           sunk_string);
                pb[s.get_x()][s.get_y()] = -2; // sunk probability
                return;
            }
        }
    }
}

void battleship::register_hit(board& hit_board, board& firing_board,
                              pboard& f_pboard, pos p) {
    hit_board[p.y][p.x]    = B_HIT_SHIP;
    firing_board[p.y][p.x] = B_HIT_SHIP;
    f_pboard[p.y][p.x]     = -1;
}

void battleship::register_miss(board& missed_board, board& firing_board,
                               pboard& f_pboard, pos p) {
    missed_board[p.y][p.x] = B_MISSED_SHOT;
    firing_board[p.y][p.x] = B_MISSED_SHOT;
    f_pboard[p.y][p.x]     = -3;
}

bool battleship::hu_make_guess(pos& p) {
    string input;
    std::cout << "Enter a coordinate (i.e. A3): ";
    getline(cin, input);
    p.y = int(toupper(input[0])) - 65;
    p.x = std::stoi(input.substr(1));
    if (pc_own_board[p.y][p.x] == B_HIT_SHIP) {
        std::cout << "Already hit there!\n";
        return false;
    }
    if (pc_own_board[p.y][p.x] == B_MISSED_SHOT) {
        std::cout << "Already missed there!\n";
        return false;
    }
    return (p.x <= 9 and p.y <= 9 and p.x >= 0 and p.y >= 0);
}

void battleship::hu_process_guess(const pos& p) {
    std::string guess;
    if (pc_own_board[p.y][p.x] == B_EMPTY_OCEAN) {
        register_miss(pc_own_board, hu_try_board, hu_pboard, p);
        guess = "\u001b[37;1m" + p.to_string() + "\u001b[0m";
        std::cout << I_MISSED_SHOT << "\n";
    } else {
        register_hit(pc_own_board, hu_try_board, hu_pboard, p);
        std::cout << "adding hit" << std::endl;
        add_hit_to_ships(hu_try_board, pc_ships, hu_pboard, p, 1, false);
        guess = "\u001b[31;1m" + p.to_string() + "\u001b[0m";
        std::cout << I_HIT_SHOT << "\n";
    }
    hu_guesses.push_back(guess);
}

void battleship::easy_guess(const board& b, pos& p) const {
    srand(time(NULL));
    p.x = rand() % 10;
    p.y = rand() % 10;
    while (b[p.y][p.x] != B_EMPTY_OCEAN) {
        p.x = rand() % 10;
        p.y = rand() % 10;
    }
}

// look for hit ships.
// if one is found, x,y will be updated accordingly, and return true.
bool battleship::search_for_hit_location(const board& b, pos& p) const {
    int converted = p.y * 10 + p.x;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i * 10 + j < converted) continue;
            if (b[i][j] == B_HIT_SHIP) {
                p.y = i;
                p.x = j;
                return true;
            }
        }
    }
    return false;
}

// checks to see if a given location is valid to fire;
bool battleship::is_valid_loc(const board& b, pos p) const {
    if (p.x > 9 || p.y > 9 || p.x < 0 || p.y < 0) return false;
    if (b[p.y][p.x] == B_EMPTY_OCEAN)
        return true;
    else
        return false;
}


// pre: a ship has already been hit.
// post: x and y are set to a new location to aim for.
bool battleship::blood_in_the_water(const board& b, pos& p) const {
    if (is_valid_loc(b, pos(p.x + 1, p.y))) {
        p.x++;
        return true;
    }
    if (is_valid_loc(b, pos(p.x - 1, p.y))) {
        p.x--;
        return true;
    }
    if (is_valid_loc(b, pos(p.x, p.y + 1))) {
        p.y++;
        return true;
    }
    if (is_valid_loc(b, pos(p.x, p.y - 1))) {
        p.y--;
        return true;
    }
    return false; // this location may not be viable (another spot on ship
                  // is...)
}

// finds an okay spot, given that a ship has been hit already
void battleship::find_medium_good_spot(const board& b, pos& p) const {
    bool valid = false;
    while (!valid) {
        search_for_hit_location(b,
                                p); // now x and y are valid coordinate of hit.
        valid = blood_in_the_water(pc_try_board, p); // if it works, we're done.
        if (!valid) { // have to increment the position for next call to search
            p.x++;
            if (p.x > 9) {
                p.x = 0;
                p.y++;
            }
        }
    }
}

// checks if a given area is clear of ships
bool battleship::check_nearby_hit(const board& board, int len, pos p,
                                  int d) const {
    if (d == 0) {
        for (int z = p.x; z < p.x + len; z++) {
            if (board[p.y][z] == B_HIT_SHIP) return true;
        }
    } else if (d == 1) {
        for (int z = p.y; z < p.y + len; z++) {
            if (board[z][p.x] == B_HIT_SHIP) return true;
        }
    }
    return false;
}

void battleship::update_probabilities(board& b, pboard& pb, pos p, int len,
                                      bool d) {
    int modifier, boundcheck;
    if (d == 0) {
        boundcheck = p.x + len - 1;
    } else {
        boundcheck = p.y + len - 1;
    }
    if (boundcheck <= 9 && check_clear_area(b, len, p, d)) {
        if (check_nearby_hit(b, len, p, d)) {
            modifier = 50;
        } else {
            modifier = 1;
        }
        for (int i = 0; i < len; i++) {
            if (d == 0)
                pb[p.y][p.x + i] += modifier;
            else
                pb[p.y + i][p.x] += modifier;
        }
    }
}

void battleship::calc_probabilities(board& b, pboard& pb) {
    int len;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) { // for each space
            pos temp(i, j);
            for (int z = 0; z < 5; z++) { // for each ship
                len = ship_len_lookup[z];
                update_probabilities(b, pb, temp, len, 0);
                update_probabilities(b, pb, temp, len, 1);
            }
        }
    }
}

void battleship::find_best_guess(board& b, pboard& pb, pos& p) {
    int largest_probability = 0;
    pos temp;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            temp.y = i;
            temp.x = j;
            if (pb[i][j] > largest_probability && is_valid_loc(b, temp)) {
                largest_probability = pb[i][j];
                p                   = temp;
            }
        }
    }
    vector<pos> possibles;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            temp.y = i;
            temp.x = j;
            if (pb[i][j] == largest_probability && is_valid_loc(b, temp)) {
                possibles.push_back(temp);
            }
        }
    }
    int ran = possibles.size();
    if (ran == 1) {
        ran = 0;
    } else {
        ran = rand() % (ran - 1);
    }
    p = possibles.at(ran);
}

string battleship::pc_make_guess() {
    pos    p;
    bool   found;
    string retstr = "";
    if (difficulty == "easy") { easy_guess(pc_try_board, p); }
    if (difficulty == "medium") {
        p.x   = 0;
        p.y   = 0;
        found = search_for_hit_location(pc_try_board, p);
        if (!found) {
            easy_guess(pc_try_board, p);
        } else {
            find_medium_good_spot(pc_try_board, p);
        }
    }
    if (difficulty == "hard") {
        init_pboard(pc_pboard);
        calc_probabilities(pc_try_board, pc_pboard);
        find_best_guess(pc_try_board, pc_pboard, p);
    }

    retstr = "PC guesses: \u001b[32;1m" + string(1, (char)(p.y + 65)) +
             to_string(p.x) + "\u001b[0m - ";
    if (hu_own_board[p.y][p.x] != B_EMPTY_OCEAN) {
        retstr += "\033[1;31mhit!\033[0m";
        pc_guesses.push_back("\u001b[31;1m" + string(1, (char)(p.y + 65)) +
                             to_string(p.x) + "\u001b[0m");
        register_hit(hu_own_board, pc_try_board, pc_pboard, p);
        add_hit_to_ships(hu_own_board, hu_ships, pc_pboard, p, 1, true);
        add_hit_to_ships(pc_try_board, hu_ships, pc_pboard, p, 0, true);
    } else {
        retstr += "\033[1;37mmiss!\033[0m";
        pc_guesses.push_back("\u001b[37;1m" + string(1, (char)(p.y + 65)) +
                             to_string(p.x) + "\u001b[0m");
        register_miss(hu_own_board, pc_try_board, pc_pboard, p);
    }
    return retstr;
}


// continue allowing the user to guess until one player wins
void battleship::run_game() {
    int turn_counter = 0;
    start_game();
    bool        valid_guess;
    pos         p;
    std::string pc_guess;
    while (!gameover()) {
        if (!hu_make_guess(p)) {
            std::cout << "Invalid guess! Please guess again.\n";
            continue;
        } else {
            hu_process_guess(p);
        }
        pc_guess = pc_make_guess();
        print_user_game_data();
        std::cout << std::endl;
        turn_counter++;
    }
    std::cout << "The game was finished in " << turn_counter << " turns."
              << std::endl;
}

// checks if a given area is clear of ships
bool battleship::check_clear_area(const board& board, int len, pos p,
                                  int d) const {
    int count = 0;
    if (d == 0) {
        for (int z = p.x; z < p.x + len; z++) {
            if (board[p.y][z] == B_EMPTY_OCEAN || board[p.y][z] == B_HIT_SHIP)
                count++;
        }
    } else if (d == 1) {
        for (int z = p.y; z < p.y + len; z++) {
            if (board[z][p.x] == B_EMPTY_OCEAN || board[z][p.x] == B_HIT_SHIP)
                count++;
        }
    }
    if (count == len)
        return true;
    else
        return false;
}

// i is ship length; x is x loc, y is y loc,
// d is direction (1 vertical, 0 horizontal)
// returns when it finds a valid placement position for the given ship
void battleship::find_valid_start_loc(const board& board, int len, pos& p,
                                      bool& d) const {
    while (true) {
        p.x = (rand() % 10);
        p.y = (rand() % 10);
        d   = rand() % 2; // 0 or 1, indicating direction
        if ((d == 0 && (p.x + len <= 10)) || (d == 1 && (p.y + len <= 10))) {
            if (check_clear_area(board, len, p, d)) { break; }
        }
    }
}

// put a ship in a board
void battleship::place_ship(board& b, int len, pos p, bool d, string s) {
    if (d == 0) {
        for (int z = p.x; z < p.x + len; z++) {
            b[p.y][z] = s;
        }
    }
    if (d == 1) {
        for (int z = p.y; z < p.y + len; z++) {
            b[z][p.x] = s;
        }
    }
}


bool battleship::game_ready(const board& human_board) const {
    int usr_star_count = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (human_board[i][j] == B_EMPTY_OCEAN) usr_star_count++;
        }
    }
    return (usr_star_count == 66);
}

bool toBool(string s);
bool toBool(string s) {
    return (s != "0");
}

// gets starting coordinate from human
void battleship::get_start_coord(const board& b, int len, pos& p,
                                 bool& d) const {
    bool   badinput = true;
    string coord;
    while (badinput) {
        std::cout << "Enter a starting coordinate (i.e. J5): ";

        getline(cin, coord);
        p.y = (int)coord[0] - 65;
        if (coord.length() > 2) {
            p.x = ((int)coord[1] - 48) * 10 + (int)coord[2] - 48;
        } else {
            p.x = (int)coord[1] - 48;
        }
        if (p.x < 0 || p.x > 9 || p.y < 0 || p.y > 9) {
            std::cout << "Bad coordinate. Please try again.\n";
            continue;
        } else
            badinput = false;
    }
    std::cout << "Enter a direction (0 for horizontal, 1 for vertical): ";
    getline(cin, coord);
    d = toBool(coord);
    std::cout << "D: " << d << std::endl;
    if ((d == 0 && (p.x + len - 1 > 9)) || (d == 1 && (p.y + len - 1 > 9))) {
        std::cout << "Ship will fall off the board. Please try again.\n";
        get_start_coord(b, len, p, d);
    }

    if (!check_clear_area(b, len, p, d)) {
        std::cout << "Already a ship there! Please try again.\n";
        get_start_coord(b, len, p, d);
    }
}

void battleship::generate_ships(board& b, vector<ship>& ships, bool hu_pc) {
    pos    p;
    int    len;
    bool   direction;
    string ship_name, ship_class;
    ship   s;
    for (int i = 0; i < 5; i++) {
        len        = ship_len_lookup[i];
        ship_class = ship_class_lookup[i];
        ship_name  = ship_name_lookup[i];
        if (hu_pc) {
            std::cout << "Placing ships of type " << ship_name
                      << " (length: " << len << ")" << std::endl;
            print_board(b);
            get_start_coord(b, len, p, direction);
        } else {
            find_valid_start_loc(b, len, p, direction);
        }
        place_ship(b, len, p, direction, ship_class);
        s = ship(ship_name, ship_class, p, len, direction);
        s.set_hits();
        ships.push_back(s);
    }
}

void print_digits();
void print_digits() {
    std::cout << "   \033[1;32m0123456789\033[0m";
    std::cout << "               \033[1;32m0123456789\033[0m\n";
}

void print_header();
void print_header() {
    std::cout << "   YOUR SHOTS               YOUR SHIPS       YOUR MOVES";
    std::cout << "    PC MOVES" << std::endl;
}

string green_char(int i);
string green_char(int i) {
    return "\033[1;32m" + string(1, char(65 + i)) + "\033[0m";
}

void battleship::print_board(const board& b) const {
    std::cout << "   Your board" << std::endl;
    std::cout << "   \033[1;32m12345678910\033[0m" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << " " << char(65 + i) << " ";
        for (int j = 0; j < 10; j++) {
            std::cout << b[i][j];
        }
        if (i < (int)hu_guesses.size() && i < (int)pc_guesses.size()) {
            std::cout << " " << green_char(i) << "         "
                      << hu_guesses[hu_guesses.size() - i - 1] << "           "
                      << pc_guesses[pc_guesses.size() - i - 1] << std::endl;
        } else {
            std::cout << " " << green_char(i) << std::endl;
        }
    }
}

void battleship::print_two_boards(const board& b1, const board& b2) const {
    print_digits();
    for (int i = 0; i < 10; i++) {
        std::cout << " " << green_char(i) << " ";
        for (int j = 0; j < 10; j++) {
            std::cout << b1[i][j];
        }
        std::cout << " " << green_char(i) << "           " << green_char(i)
                  << " ";

        for (int j = 0; j < 10; j++) {
            std::cout << b2[i][j];
        }
        if (i < (int)hu_guesses.size() && i < (int)pc_guesses.size()) {
            std::cout << " " << green_char(i) << "         "
                      << hu_guesses[hu_guesses.size() - 1 - i] << "           "
                      << pc_guesses[pc_guesses.size() - 1 - i] << std::endl;
        } else {
            std::cout << " " << green_char(i) << std::endl;
        }
    }
    print_digits();
}

void battleship::print_user_game_data() const {
    print_header();
    print_two_boards(hu_try_board, hu_own_board);
}


void battleship::print_full_game_data(string outstr) const {
    std::cout << outstr << std::endl;
    std::cout << "    MY  SHIPS                MY  SHOTS" << std::endl;
    print_two_boards(pc_own_board, pc_try_board);
}
