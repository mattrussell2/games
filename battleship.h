#include<iostream>
#include<string>
#include<cstdlib>
#include<vector>
#include "ship.h"
using namespace std;

typedef vector< vector<string> > board;

class battleship{
 public:
  battleship();
  ~battleship();
  
  void print_game() const;
  void run_game();
  
 private: 
  void init_board(board *&board);
  
  bool is_valid_loc(board *b,int x, int y) const;
  bool blood_in_the_water(board *b,int &x, int &y) const;
  bool search_for_hit_location(board *b, int &x, int &y) const;

  void medium_guess(board *b, int &x, int &y) const; //still to be implemented
  void easy_guess(board *b, int &x, int &y) const;// random guesses

  void get_start_coord(board *b, int len, int &x, int &y, bool &d) const;
  void generate_ships(board *&b,vector<ship> *&ships);
  void human_generate_ships(board *&b,vector<ship> *&ships);
  void find_valid_start_loc(board *b,int len, int &x, int &y, bool &d) const;
  bool check_clear_area(board *b,int len, int x, int y, int d) const;
  void place_ship(board *&b,int len, int x, int y, bool d,string s);

  void register_hit(board *&hit_board,board *&fire_board,int x, int y);
  void register_miss(board *&missed_board,board *&f_board,int x, int y);

  bool game_ready(board *human_board) const; 
  bool gameover() const;

  //these two functions make the guesses and call register hits/miss fns.
  void hu_make_guess();
  void pc_make_guess();

  //first board is the TRY board that we will add the letters to if a ship is sunk
  void add_hit_to_ships(board *&b,vector<ship> *&ships, int x, int y, bool disp);
  
  //searches through the ships to find which one is sunk
  void check_sunken_ships(board *&hit_board,vector<ship>*ships) const;

  void print_board(board *b) const; 
  //void print_boards(vector<string> *, vector<string> *) const;
  void print_user_game_data() const;
  //void print_cheater_game_data() const;

  board *pc_own_board;
  board *pc_try_board;
  board *hu_own_board;
  board *hu_try_board;
 
  int ship_len_lookup[5] = {5,4,3,3,2};
  char ship_class_lookup[5] = {'A','B','C','S','D'};
  string ship_name_lookup[5] = {"Aircraft Carrier", "Battleship", "Crusier",
				"Submarine","Destroyer"};
  vector<ship> *pc_ships;
  vector<ship> *hu_ships;
  
  string difficulty;

};
