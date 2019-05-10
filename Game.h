#ifndef GAME_H
#define GAME_H
#include "Board.h"

class Game{
 public:
  Game() { b = new Board(); first_move = true; curr_taken = new bool[2] {false, false};};
  ~Game(){ delete b; delete [] curr_taken; delete [] dice; };
  void run_game();
  void welcome_message();
  bool check_gameover(){ return b->check_gameover(); }
  bool check_move(bool print, int start, int end);  
  bool determine_valid_moves(string color);
  void get_move();
  void make_move();
  void take_turn();  
  void determine_first_turn();
  bool test_in_zone(string color);
  bool check_zone_move(bool print, int start, int end);
  bool validate_input();
 private:
  int first_pos, second_pos;
  string strin1, strin2;
  bool first_move;
  string turn;

  //dynamically allocated
  Board *b;
  int *dice;
  bool *curr_taken;
  //  bool black_in_zone, white_in_zone;
};

#endif
