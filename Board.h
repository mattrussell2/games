#include<iostream>
#include<string>
#include<random>
#ifndef BOARD_H
#define BOARD_H
#include<time.h>
#include<sstream>

using namespace std;

//todo: implement knocks,  endgame, and AI.
const int PIPS = 15;

class Board{

 public:
  Board();
  ~Board();
  int* roll_dice();
  void print_board();
  bool test_space_empty(int space);
  string color_on_space(int space);
  int num_pips_on_space(int space);
  void remove_one_from_space(int space);
  void add_one_to_space(int space,string color);
  bool hitable(int space);
  void print_space(int space);
  void add_knocked(string color);
  void remove_knocked(string color);
  int get_knocked(string color);
  void add_one_off_board(string color);
  bool check_gameover() { return (off_black == PIPS || off_white == PIPS); };//(off_black == 15 || off_white == 15); };
  bool white_win() { return   (off_white == PIPS); };//(off_white == 15); };
 private:
  string **board;
  int *dice;
  int double_cube;
  int knocked_black;
  int knocked_white;
  int off_black;
  int off_white;
};
#endif
