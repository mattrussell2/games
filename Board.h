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
  void reset();
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
  void set_score(int w, int b){ white_score = w; black_score = b;} 
  int any_black_in_white_home();
  int any_white_in_black_home();
  int num_off(string color) { if (color == "white") return off_white; else return off_black; };
  int num_knocked(string color) { if (color == "white") return knocked_white; else return knocked_black; };

  int double_amount(){ return double_cube; }
  void set_double(int d) { double_cube = d; };
  void decline_double(string color) {if (color=="white") off_black = PIPS; else off_white = PIPS; };
  
 private:
  string **board;
  int *dice;
  int double_cube;
  int knocked_black;
  int knocked_white;
  int off_black;
  int off_white;
  int white_score;
  int black_score;
};
#endif
