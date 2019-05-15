#ifndef GAME_H
#define GAME_H
#include "Board.h"

class Game{
 public:
  Game() { double_turn = "empty"; first_move = true; curr_taken = new bool[2] {false, false}; white_score = 0; black_score = 0; double_declined = false; };
  ~Game(){ delete b; delete [] curr_taken; delete [] dice; };
  void run_game();
  void welcome_message();
  bool check_gameover(){ return b->check_gameover(); }
  bool check_move(bool print, int start, int end);  
  bool determine_valid_moves(string color);
  bool get_move();
  void make_move();
  void take_turn();  
  void determine_first_turn();
  bool test_in_zone(string color);
  bool check_zone_move(bool print, int start, int end);
  bool validate_input();
  bool taken_turn(int start, int end); //test if this turn was taken.
  bool double_cube();
  int white_score;
  int black_score;

 private:
  int first_pos, second_pos;
  string strin1, strin2;
  bool first_move;
  string turn;
  bool double_declined;

  //dynamically allocated
  Board *b;
  int *dice;
  bool *curr_taken;

  string double_turn;
  //  bool black_in_zone, white_in_zone;
};

#endif
