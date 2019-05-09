#include "Board.h"

using namespace std;

Board::Board(){
  board = new string*[24];
  for (int i=0; i<24; i++){
    board[i] = new string[15];
    for (int j=0; j<15; j++){
      board[i][j] = "empty";
    }
  }
  for (int i=0;i<2;i++){
     board[0][i] =  "black";  
     board[23][i] = "white";
  }
  
  for (int i=0;i<3;i++){   
     board[7][i] =  "white";
     board[16][i] = "black";
  }
  
  for (int i=0;i<5;i++){
     board[5][i] =  "white";
     board[11][i] = "black";
     board[12][i] = "white";
     board[18][i] = "black";
  }
  dice = new int[2];
  double_cube = 0;
  knocked_black = 0;
  knocked_white = 0;
};



int* Board::roll_dice(){
  dice[0] = rand() % 6 + 1;
  dice[1] = rand() % 6 + 1;
  return dice;
}


Board::~Board(){
  for (int i=0;i<25;i++){
    delete [] board[i];
    board[i] = nullptr;
  }
  delete [] board;
};

bool Board::test_space_empty(int space){
  if (board[space-1][0] == "empty") return true;
  return false;
}

string Board::color_on_space(int space){
  return board[space-1][0];
}

bool Board::hitable(int space){
  return (num_pips_on_space(space) == 1);
}

int Board::num_pips_on_space(int space){
  int num = 0;
  for (int i=0;i<15;i++){
    if (board[space-1][i] == "empty") break;
    else num++;
  }
  return num;
}

void Board::print_space(int space){
  for (int i=0;i<15;i++){
    cout << board[space-1][i] << " ";
  }
  cout << endl;

}
void Board::remove_one_from_space(int space){ 
  int loc = num_pips_on_space(space);  
  board[space-1][loc-1] = "empty";
}

void Board::add_one_to_space(int space, string color){
  int loc = num_pips_on_space(space);
  board[space-1][loc] = color;
}

void Board::add_knocked(string color){
  if (color == "black") knocked_black++;
  else                  knocked_white++;
}

void Board::remove_knocked(string color){
 if (color == "black") knocked_black--;
 else                  knocked_white--;
}

int Board::get_knocked(string color) {
  if (color == "white") return knocked_white;
  else return knocked_black;
}

void Board::print_board() {
  int boardindex;
  cout << endl;
  cout << "    || ";
  for (int i=13;i<25;i++){    
    cout << " " << i << "   ";
    if (i==18) cout << "   ";
  } cout << "||" << endl;

  cout << "    ||";
  for (int i=0; i<76; i++){
    cout << "=";  
  } cout << "||" << endl;

  for (int row=0; row<12; row++){
    cout << "    || ";
    if (row < 5) { boardindex = 12; }
    else         { boardindex = 11; }
    for (int col=0; col<13; col++){
      if ((row != 5 && row != 6) && col == 6) { cout << "|| "; }
      else if (row < 5) {
	if (board[boardindex][row] == "empty"){ cout << "  |   "; }
	else { cout << board[boardindex][row] << " "; }
	boardindex++;       
      } else if (row > 6){	
	if (board[boardindex][11 - row] == "empty"){ cout << "  |   "; }
	else { cout << board[boardindex][11 - row] << " "; }
	boardindex--;
      }
    }
    if (row == 5 || row == 6){
      int space;      
      for (int i=0;i<12;i++){
	if (row == 5) space = 13 + i; 
	else          space = 12 - i;
	if (num_pips_on_space(space) > 5){
	  cout << " +";
	  cout << num_pips_on_space(space) - 5;
	  if (num_pips_on_space(space) > 9){
	    cout << "  ";
	  }else cout << "   ";
	}else {
	  cout << "      ";	  
	}
	if (i==5){
	  if (row == 5 && knocked_black > 0) { cout << "b" << knocked_black << " "; }
	  else if (row == 6 && knocked_white > 0) { cout << "w" << knocked_black << " "; }
	  else cout << "   ";
	}
      }
    }
    cout << "||" << endl;
  }
   cout << "    ||";
  for (int i=0; i<76; i++){
    cout << "=";  
  }
  cout << "||" << endl;
  cout << "    || ";
  for (int i=12;i>0;i--){
    if (i < 10) cout << " ";
    cout << " " << i << "   ";
    if (i==7) cout << "   ";
  }
  cout << "||" << endl;
  cout << endl;
}

int main(){
  Game *g = new Game();
  g->run_game();
  return 0;
}
