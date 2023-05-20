#include "Game.h"
#include "Board.h"
/* todo: clean things up! */

using namespace std;

void Game::run_game() {
    b.set_score(white_score, black_score);
    b.print_board();
    determine_first_turn();
    double_declined = false;
    double_turn     = "any";
    first_move      = true;
    string s;
    while (!gameover()) {
        take_turn();
        if (gameover()) break;
        if (double_turn == "any" or double_turn == turn) {
            cout << turn << "\'s turn! press <enter> to roll, or d to double. ";
            getline(cin, s, '\n');
            if (s[0] == 'd') double_cube();
        } else {
            cout << turn << "\'s turn! press <enter> to roll.";
            getline(cin, s, '\n');
        }
    }

    end_game(b.get_winner());
    b.set_score(white_score, black_score);
    cout << "thanks for playing." << endl;
}

void Game::end_game(string winner) {
    string loser;
    bool   in_home_test;
    if (winner == "white") {
        loser        = "black";
        in_home_test = b.any_black_in_white_home();
    } else {
        loser        = "white";
        in_home_test = b.any_white_in_black_home();
    }
    int score_to_add = 0;
    cout << winner << " wins!" << endl;
    if (!double_declined) {
        int  num_off = b.num_off(loser);
        bool gammon  = num_off == 0;
        bool backgammon =
            (gammon and (b.num_knocked(loser) > 0 or in_home_test));
        if (backgammon) {
            cout << "it's a backgammon!" << endl;
            score_to_add = b.double_amount() * 3;
        } else if (gammon) {
            cout << "it's a gammon!" << endl;
            score_to_add = b.double_amount() * 2;
        } else {
            score_to_add = b.double_amount();
        }
    } else {
        score_to_add = b.double_amount();
    }
    winner == "white" ? white_score += score_to_add
                      : black_score += score_to_add;
}

bool Game::test_in_zone() {
    if (b.get_knocked(turn) > 0) return false;
    int curr, end;
    if (turn == "white") {
        curr = 7;
        end  = 25;
    } else {
        curr = 1;
        end  = 19;
    }
    while (curr < end) {
        if (b.color_on_space(curr) == turn) return false;
        curr++;
    }
    return true;
}

bool Game::check_zone_move(int start, int end, bool print) {
    if ((turn == "white" and start > 6) or (turn == "black" and start < 19)) {
        if (print)
            cout << "coordinates out of bounds. please try again." << endl;
        return false;
    }

    int difference = abs(start - end);

    if (end == 0 or end == 25) {

        // move off the board is exact dice roll.
        if ((!move_taken.first  and difference == dice.first) or
            (!move_taken.second and difference == dice.second))
            return true;

        int curr;
        if (end == 0)
            curr = 6;
        else
            curr = 19;
        while (curr != start) {
            if (b.num_pips_on_space(curr) > 0 and
                b.color_on_space(curr) == turn) {
                if (((turn == "white") and
                     (check_move(curr, curr - dice.first) or
                      check_move(curr, curr - dice.second))) or
                    ((turn == "black") and
                     (check_move(curr, curr + dice.first) or
                      check_move(curr, curr + dice.second)))) {
                    if (print)
                        cout << "a higher piece can be moved first" << endl;
                    return false;
                }
            }
            turn == "white" ? curr-- : curr++;
        }
    }
    return true;
}

bool Game::check_move(int start, int end, bool print) {
    if (start < 0 or start > 25 or (!test_in_zone() and (end < 0 or end > 25))) {
        if (print)
            cout << "coordinate out of bounds. please try again." << endl;
        return false;
    }

    if (start == end) {
        if (print) cout << "start and end are same position" << endl;
        return false;
    }

    if (b.get_knocked(turn) == 0 and (start == 0 or start == 25)) {
        if (print) cout << "cannot start there\n";
        return false;
    }

    if (b.get_knocked(turn) > 0 and ((turn == "black" and start != 0) or 
                                     (turn == "white" and start != 25))) {
        if (print)
            cout << "remove your piece(s) from the bar before continuing.\n";
        return false;
    }

    if ((start > end and turn == "black") or
        (start < end and turn == "white")) {
        if (print) cout << "wrong direction. please try again." << endl;
        return false;
    }

    if (start > 0 and start < 25 and b.color_on_space(start) != turn) {
        if (print) cout << "wrong starting color. please try again.\n";
        return false;
    }

    if (end > 0 and end < 25 and b.num_pips_on_space(end) > 1 and
                                 b.color_on_space(end) != turn) {
        if (print) cout << "cannot move pip there. please try again.\n";
        return false;
    }

    int difference = abs(start - end);
    if (end > 0 and end < 25 and difference != dice.first and
        difference != dice.second) {
        if (print) cout << "incorrect move. please try again\n";
        return false;
    }

    if (((difference > dice.first and difference > dice.second) or
         (move_taken.first and difference > dice.second) or
         (move_taken.second and difference > dice.first))) {
        if (print) cout << "move is too large. please try again." << endl;
        return false;
    }

    if (dice.first != dice.second and
        ((difference == dice.first and move_taken.first) or
         (difference == dice.second and move_taken.second)) and
        (!test_in_zone() and end != 25 and end != 0)) {
        if (print) cout << "already moved that die. please try again.\n";
        return false;
    }

    if (test_in_zone()) return check_zone_move(start, end, print);

    if (end == 25 or end == 0) {
        if (print) cout << "cannot take pieces off board yet!" << endl;
        return false;
    }

    if (abs(start - end) != dice.first and abs(start - end) != dice.second) {
        if (print) cout << "incorrect move. please try again\n" << endl;
        return false;
    }

    return true;
}

bool Game::determine_valid_moves(bool debug) {
    for (int i = 0; i < 26; i++) {
        if ((turn == "white" and (check_move(i, i - dice.first, debug) or
                                  check_move(i, i - dice.second, debug))) or
            (turn == "black" and (check_move(i, i + dice.first, debug) or
                                  check_move(i, i + dice.second, debug))))
            return true;
    }
    if (debug) cout << "NO VALID MOVES" << endl;
    return false;
}

void Game::make_move(int first_pos, int second_pos) {
    if (first_pos == 0) b.remove_knocked("black");
    if (first_pos == 25) b.remove_knocked("white");

    if (second_pos > 0 and second_pos < 25) {
        // deal with a knock
        if (b.hitable(second_pos) and turn != b.color_on_space(second_pos)) {
            b.remove_one_from_space(second_pos);
            cout << "knocked!" << endl;
            b.add_knocked(othercolor(turn));
        }
    }
    if (first_pos != 25 and first_pos != 0) b.remove_one_from_space(first_pos);
    if (second_pos < 25 and second_pos > 0)
        b.add_one_to_space(second_pos, turn);
    else
        b.add_one_off_board(turn);
}

bool Game::validate_input(const std::string& strin) const {
    for (int i = 0; i < strin.size(); i++) {
        if (!std::isdigit(strin[i])) return false;
    }
    return true;
}

bool Game::double_cube() {
    string opponent, accept;
    opponent = othercolor(turn);
    cout << turn << " has offered a double. " << opponent
         << ", do you accept? (y/n) " << endl;
    getline(cin, accept);
    while (accept != "y" and accept != "n") {
        cout << "please enter y/n" << endl;
        getline(cin, accept);
    }
    if (accept == "y") {
        b.set_double(b.double_amount() * 2);
        double_turn = othercolor(turn);
        cout << "double accepted." << endl;
        cout << turn << " to move" << endl;
        return true;
    } else {
        b.decline_double(othercolor(turn));
        double_declined = true;
        return false;
    }
}

Move Game::get_move() {
    std::string strin1, strin2;
    cin >> strin1 >> strin2;
    getchar();
    while (!validate_input(strin1) or !validate_input(strin2)) {
        cout << "not valid numerical input. please try again." << endl;
        cin >> strin1 >> strin2;
        getchar();
    }
    Move m;
    m.first  = stoi(strin1);
    m.second = stoi(strin2);
    return m;
}

void Game::take_turn() {
    if (!first_move) {
        roll_dice();
        cout << "roll is: " << dice.first << " " << dice.second << endl;
    } else
        first_move = false;

    move_taken.first  = false;
    move_taken.second = false;

    int stopval = 2;
    if (dice.first == dice.second) {
        cout << "DOUBLES! player has 4 moves" << endl;
        stopval = 4;
    }

    if (!determine_valid_moves()) {
        cout << "player has no valid moves." << endl;
        stopval = 0;
    }

    for (int i = 0; i < stopval; i++) {
        if (gameover()) return;
        if (stopval == 4 and i == 2) {
            move_taken.first  = false;
            move_taken.second = false;
        }
        if (!determine_valid_moves()) {
            cout << "player has no valid moves." << endl;
            break;
        }
        Move m = get_move();
        while (!check_move(m.first, m.second, true)) {
            m = get_move();
        }
        make_move(m.first, m.second);

        int difference = abs(m.second - m.first);
        if (difference == dice.first)
            move_taken.first = true;
        else if (difference == dice.second)
            move_taken.second = true;
        else { // now in zone. assume move legit, so it's gotta be the higher
               // one.
            if (dice.first > dice.second)
                move_taken.first = true;
            else
                move_taken.second = true;
        }

        b.print_board();
        if (i < stopval - 1)
            cout << "move made. [dice read: " << dice.first << " "
                 << dice.second << "]\n";
    }
    turn = othercolor(turn);
}

void Game::determine_first_turn() {
    roll_dice();

    while (dice.first == dice.second) {
        cout << "white rolls " << dice.first << endl;
        cout << "black rolls " << dice.second << endl;
        cout << "It's a tie! Press <enter> to reroll" << endl;
        cin.get();
        roll_dice();
    }
    cout << "white rolls " << dice.first << endl;
    cout << "black rolls " << dice.second << endl;
    if (dice.first > dice.second) {
        cout << "white goes first!" << endl;
        turn = "white";
    } else {
        cout << "black goes first!" << endl;
        turn = "black";
    }

    cout << "[dice read: " << dice.first << " " << dice.second << "]" << endl;
};

std::pair<int, int> Game::roll_dice() {
    dice.first  = rand() % 6 + 1;
    dice.second = rand() % 6 + 1;
    return dice;
}

int main() {
    Game g;

    cout << "welcome to backgammon!" << endl;
    cout << "moves are made one by one. " << endl;
    cout << "format: firstpos <space> secondpos <enter>. " << endl;
    cout << "use the 0 and 25 positions for the bar/off board." << endl;
    cout << "to double, enter d instead of <enter> to roll dice" << endl;
    cout << "press <enter> to auto-roll for first move." << endl;
    cin.get();

    string yn;
    while (true) {
        cout << "new game!" << endl;
        g.run_game();
        std::cout << g.report_score() << std::endl;
        cout << "would you like to keep playing? (y/n)" << endl;
        getline(cin, yn);
        while (yn[0] != 'y' and yn[0] != 'n') {
            cout << "please enter y or n" << endl;
            getline(cin, yn);
        }
        if (yn[0] == 'n') break;
    }
    std::cout << g.report_winner() << std::endl;
    cout << "\nThanks for playing :)" << endl;
    return 0;
}
