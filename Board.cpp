#include "Board.h"

using namespace std;

Board::Board(){
  board = new string*[24];
  for (int i = 0; i < 24; i++){
    board[i] = new string[15];
    for (int j = 0; j < 15; j++){
      board[i][j] = "empty";
    }
  }
  for (int i = 0; i < 2; i++){
    board[0][i] =  "\033[0;42;30mblack\033[1;42;37m";  
    board[23][i] = "white";
  }
  
  for (int i = 0; i < 3; i++){   
    board[7][i] =  "white";
    board[16][i] = "\033[0;42;30mblack\033[1;42;37m";
  }
  
  for (int i = 0; i < 5; i++){
    board[5][i] =  "white";
    board[11][i] = "\033[0;42;30mblack\033[1;42;37m";
    board[12][i] = "white";
    board[18][i] = "\033[0;42;30mblack\033[1;42;37m";
  }
  dice = new int[2];
  double_cube = 1;
  knocked_black = 0;
  knocked_white = 0;
  off_black = 0;
  off_white = 0;
  white_score = 0;
  black_score = 0;
  srand(time(NULL)); 
};

int* Board::roll_dice(){
  dice[0] = rand() % 6 + 1;
  dice[1] = rand() % 6 + 1;
  return dice;
}

Board::~Board(){
  for (int i = 0; i < 25; i++){
    delete [] board[i];
    board[i] = nullptr;
  }
  delete [] board;
};

bool Board::test_space_empty(int space){
  if (board[space - 1][0] == "empty") return true;
  return false;
}

string Board::color_on_space(int space){
  if (board[space - 1][0] == "white" or board[space - 1][0] == "empty")
    return board[space - 1][0];
  else
    return "black";
}

bool Board::hitable(int space){
  return (num_pips_on_space(space) == 1);
}

int Board::num_pips_on_space(int space){
  int num = 0;
  for (int i = 0; i < 15; i++){
    if (board[space - 1][i] == "empty") break;
    else num++;
  }
  return num;
}

void Board::print_space(int space){
  for (int i = 0; i < 15; i++){
    cout << board[space - 1][i] << " ";
  }
  cout << endl;

}
void Board::remove_one_from_space(int space){ 
  int loc = num_pips_on_space(space);  
  board[space - 1][loc - 1] = "empty";
}

void Board::add_one_to_space(int space, string color){
  int loc = num_pips_on_space(space);
  if (color == "white") board[space - 1][loc] = color;
  else board[space - 1][loc] = "\033[0;42;30mblack\033[1;42;37m";
}
void Board::add_one_off_board(string color){
  if (color == "white") off_white++; 
  else off_black++;
}
void Board::add_knocked(string color){
  if (color == "white") knocked_white++;
  else knocked_black++;
}

void Board::remove_knocked(string color){
  if (color == "white") knocked_white--;
  else knocked_black--;
}

int Board::get_knocked(string color) {
  if (color == "white") return knocked_white;
  else return knocked_black;
}

void Board::print_board() {
  int boardindex;
  cout << endl;
  cout << "\33[1;42;37m    || ";
  
  //print numbers at the top
  for (int i = 13; i < 25; i++){    
    cout << " " << i << "   ";
    if (i == 18) cout << "   ";
  } cout << "||    " << endl;
  
  //print row of == signs
  cout << "    ||";
  for (int i = 0; i < 76; i++){
    cout << "=";  
  } cout << "||    " << endl;

  //print the pieces in the board
  for (int row = 0; row < 12; row++){
    if (row == 5){
      cout << "\033[1;42;37mD:" << double_cube;
      if (double_cube < 10) cout << " ";
      cout << "|| ";
    }else if (row == 3){
      cout << "\033[1;42;37mW:" << white_score;
      if (white_score < 10) cout << " ";
      cout << "|| ";
    }else if (row == 7){
      cout << "\033[0;42;30mB:" << black_score;
      if (black_score < 10) cout << " ";
      cout << "\033[1;42;37m|| ";
    }
    else {
      cout << "    || ";
    }
    if (row < 5) { boardindex = 12; }
    else         { boardindex = 11; }
    for (int col = 0; col < 13; col++){
      if ((row != 5 && row != 6) && col == 6) { cout << "|| "; }
      else if (row < 5) {
	if (board[boardindex][row] == "empty"){
	  cout << "  |   ";
	}
	else { cout << board[boardindex][row] << " "; }
	boardindex++;       
      } else if (row > 6){	
	if (board[boardindex][11 - row] == "empty"){
	  cout << "  |   ";	 
	}
	else { cout << board[boardindex][11 - row] << " "; }
	boardindex--;
      }
    }
    if (row == 5 || row == 6){
      int space;      
      for (int i = 0; i < 12; i++){
	if (row == 5) { space = 13 + i; }
	else          { space = 12 - i; }
	if (num_pips_on_space(space) > 5){
	  if (color_on_space(space) == "white")
	    cout << " +";
	  else
	    cout << "\033[0;42;30m +";
	  cout << num_pips_on_space(space) - 5;
	  cout << "\033[1;42;37m";
	  if (num_pips_on_space(space) > 14){
	    cout << "  ";
	  }else cout << "   ";
	}else {
	  cout << "      ";	  
	}
	if (i==5){
	  if (row == 5 && knocked_white > 0) { cout << "\033[1;42;37mw" << knocked_white << " "; }
	  else if (row == 6 && knocked_black > 0) { cout << "\033[0;42;30mb" << knocked_black << "\033[1;42;37m "; }
	  else cout << "   ";
	}
      }
    }
    if (row == 5) {
      cout << "||" << "\033[0;42;30m b" << off_black << "\033[1;42;37m";
							if (off_black < 10) cout << " ";
										   cout << endl;
									  
    }else if (row == 6){
	cout << "||" << "\033[1;42;37m w" << off_white;
	if (off_white < 10) cout << " ";
	cout << endl;
      }
      else {
	cout << "||    " << endl;
      }
  }

  //print the row of == at the bottom
  cout << "    ||";
  for (int i = 0; i < 76; i++){
    cout << "=";  
  }
  cout << "||    " << endl;

  //print the numbers at the bottom
  cout << "    || ";
  for (int i = 12; i > 0; i--){
    if (i < 10) cout << " ";
    cout << " " << i << "   ";
    if (i == 7) cout << "   ";
  }
  cout << "||    \033[0m" << endl;
  cout << endl;
};

int Board::any_black_in_white_home(){
  for (int i = 1; i < 7; i++){
    if (color_on_space(i) == "black")
      return true;
  }
  return false;
};

int Board::any_white_in_black_home(){
  for (int i = 19; i < 25; i++){
    if (color_on_space(i) == "white")
      return true;
  }
  return false;
};
