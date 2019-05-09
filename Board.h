#include<iostream>
#include<string>
#include<random>
#include<sstream>

using namespace std;

//todo: implement knocks,  endgame, and AI.

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
  //  bool determine_valid_moves(string color);
  
 private:
  string **board;
  int *dice;
  int double_cube;
  int knocked_black;
  int knocked_white;
};


class Game{
 public:
  Game() { b = new Board(); first_move = true; curr_taken = new bool[2] {false, false};};
  ~Game(){ delete b; delete [] curr_taken; delete [] dice;};
  void run_game(){
    welcome_message();
    b->print_board();
    determine_first_turn();
    while(!check_gameover()){
      take_turn();
      cout << turn << "\'s turn! press <enter> to roll.";
      cin.get();      
    }
  }
  void welcome_message(){
    cout << "welcome to backgammon!" << endl;
    cout << "i assume you know the rules. " << endl;
    cout << "press <enter> to roll for first move." << endl;
    cin.get();
  }

  bool check_gameover(){
    return false;
  }

  bool check_move(bool print,int start, int end){
    if (start < 0 || start > 25) {
      if (print)  cout << "coordinate is out of bounds. please try again." << endl;
      return false;
    }    
    else if ((b->get_knocked("black") == 0 && start == 0) || (b->get_knocked("white") == 0 && start == 25)){	
      if (print) cout << "faulty starting coordinates. black on bar starts at 0; white on bar starts at 25. please try again." << endl;
    return false;
    }
    if (start != 0 && start != 25){
      if(b->test_space_empty(start)){
	if (print) cout << "board is empty at that starting position. please try again." << endl;      
	return false;
      }
      if(((turn == "black") && (b->color_on_space(start) == "white")) ||
       ((turn == "white") && (b->color_on_space(start) == "black"))) {
	if (print) cout << "wrong starting color at that location. please try again." << endl;
      return false;
      }
    }

    if (((start > end) && (turn == "black")) ||
	((start < end) && (turn == "white"))) {
      cout << "wrong direction. please try again." << endl;
      return false;
    }  

    if((((turn == "black") && (b->color_on_space(end) == "white")) ||
	((turn == "white") && (b->color_on_space(end) == "black"))) &&
	  (b->num_pips_on_space(end) > 1)) {
      cout << "cannot move pip there; location is unhitable. please try again." << endl;
      return false;
    }
    if (((start < end) &&
	 ((end - start != dice[0]) &&
	  (end - start != dice[1]))) ||
	((start > end) &&
	 ((start - end != dice[0]) &&
	  (start - end != dice[1])))){
      if (print) cout << "incorrect move given your dice roll. please try again" << endl;
      return false;
    }
    if (
	(((start < end) &&
	  (end - start == dice[0]) &&
	  (dice[0] != dice[1]) &&
	  (curr_taken[0] == true)) ||
	 ((start < end) &&
	  (end - start == dice[1]) &&
	  (dice[0] != dice[1]) &&
	  (curr_taken[1] == true))) ||
	(((start > end) &&
	  (start - end == dice[0]) &&
	  (dice[0] != dice[1]) &&
	  (curr_taken[0] == true)) ||
	 ((start > end) &&
	  (start - end == dice[1]) &&
	  (dice[0] != dice[1]) &&
	  (curr_taken[1] == true)))
	){
      if (print) cout << "you have already moved that die. please try again" << endl;
      return false;
    }
    if ((turn == "black" && b->get_knocked("black") > 0 && start != 0) ||
	(turn == "white" && b->get_knocked("white") > 0 && start != 25)) {
      if (print) cout << "you must remove your piece(s) from the bar before continuing." << endl;
      return false;
    }
    return true;
  }

  
  bool determine_valid_moves(string color){
    if (color == "black"){   
      for (int i=0;i<20;i++){
	if (check_move(false, i,i+dice[0]) || check_move(false, i,i+dice[1])) return true;
      }
      return false;
    }

    else {
       for (int i=25;i<5;i++){
	 if (check_move(false, i,i-dice[0]) || check_move(false, i,i-dice[1])) return true;
       }
      return false;
    }
  }

  void get_move(){    
    cin >> first_pos >> second_pos;
    cin.get();
  }

  void make_move(){
    if (first_pos == 25) { b->remove_knocked("black"); }
    if (first_pos == 0)  { b->remove_knocked("white"); }
    
    //deal with a knock
    if (b->hitable(second_pos) && b->color_on_space(second_pos) != turn){
      if (first_pos != 25 && first_pos != 0)
	b->remove_one_from_space(second_pos);
      cout << "knocked!" << endl;
      if (turn == "white") b->add_knocked("black");
      else                 b->add_knocked("white");
    }
    if (first_pos != 25 && first_pos != 0)
      b->remove_one_from_space(first_pos);    
    b->add_one_to_space(second_pos,turn);
  }
  
  void take_turn(){
    if (!first_move) {
      dice = b->roll_dice();
      cout << "roll is: " << dice[0] << " " << dice[1] << endl;
    } else first_move = false;

    curr_taken[0] = false;
    curr_taken[1] = false;

    int stopval = 2;   
    if (dice[0] == dice[1]){
      cout << "DOUBLES! player has 4 moves" << endl;
      stopval = 4;
    }
    if (!determine_valid_moves(turn)){
      cout << "player has no valid moves." << endl;
      stopval = 0;
    }

    for (int i=0;i<stopval;i++){
      get_move();
      while(!check_move(true, first_pos, second_pos)){
	get_move();	
      }
      make_move();
      if (((first_pos < second_pos) && (second_pos - first_pos) == dice[0]) ||
	  ((first_pos > second_pos) && (first_pos - second_pos) == dice[0])) {
	  curr_taken[0] = true;
      }
      b->print_board();
      if (i==0) cout << "move made. [dice read: " << dice[0] << " " << dice[1] << "]" << endl;      
    }        
    if (turn == "white") turn = "black";
    else                 turn = "white";   
  }
  
  
  void determine_first_turn(){   
    dice = b->roll_dice();
    while (dice[0] == dice[1]){
      cout << "White rolls: " << dice[0] << endl;
      cout << "Black rolls: " << dice[1] << endl;
      cout << "It's a tie! Press <enter>  to reroll" << endl;
      cin.get();
      dice = b->roll_dice();
    }
    cout << "White rolls " << dice[0] << endl;
    cout << "Black rolls " << dice[1] << endl;
    if (dice[0] > dice[1]){
      cout << "White goes first!" << endl;
      turn = "white";
    }else  {
      cout << "Black goes first!" << endl;
      turn = "black";
    }
    cout << "make your moves, one at a time." << endl << "format: firstpos <space> secondpos <enter>" << endl;        
  };
 private:
  int first_pos, second_pos;
  bool first_move;
  string turn;

  //dynamically allocated
  Board *b;
  int *dice;
  bool *curr_taken;
};
