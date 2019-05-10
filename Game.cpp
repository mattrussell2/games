#include "Game.h"
#include "Board.h"

void Game::run_game()
{
  welcome_message();
  b->print_board();
  determine_first_turn();
  while(!check_gameover())
    {
      take_turn();
      cout << turn << "\'s turn! press <enter> to roll.";
      cin.get();      
    }
  if (b->white_win()) cout << "White Wins!" << endl;
  else cout << "Black Wins!" << endl;
  cout << "thanks for playing." << endl;  
}

void Game::welcome_message()
{  
  cout << "welcome to backgammon!" << endl;
  cout << "i assume you know the rules. " << endl;
  cout << "press <enter> to roll for first move." << endl;
  cin.get();
}

bool Game::test_in_zone(string color){
  if (color == "white"){
    if (b->get_knocked("white") > 0) return false;
    for (int i = 24; i > 6; i--){
      if (b->color_on_space(i) == "white") return false;
    }
    return true;
  } else {
    if (b->get_knocked("black") > 0) return false;
    for (int i = 1; i < 19; i++){
      if (b->color_on_space(i) == "\033[0;42;30mblack\033[1;42;37m") return false;      
    }
    return true;
  }
}

//we know all pieces are in the zone. if we have a move within the board, we have 
//to take it
bool Game::check_zone_move(bool print, int start, int end){  
  if (turn == "white"){
    if (b->color_on_space(start) != "white") return false;//continue;    
    int difference = start - end;
    if (end <= 0 && (difference <= dice[0] || difference <= dice[1])) return true;
    if (end > 0){
      string c =  b->color_on_space(end);
      if (c=="empty" || c == "white") return true;
    }
    return false;
  }   
  else {
    if (b->color_on_space(start) != "\033[0;42;30mblack\033[1;42;37m") return false;//continue;    
    int difference = end - start;
    if (end >= 25 && (difference <= dice[0] || difference <= dice[1])) return true;
    if (end < 25){
      string c =  b->color_on_space(end);
      if (c=="empty" || c == "\033[0;42;30mblack\033[1;42;37m") return true;
    }
    return false;
  }
}
  /*if (turn == "white"){
        for (int i=6;i>0;i--) {
      if (b->color_on_space(i) != "white") continue;
      for (int j=0;j<2;j++){
	if (!curr_taken[j]){	  
	  if (i - dice[j] <= 0) return true;
	  string c =  b->color_on_space(i - dice[j]);
	  if (c=="empty" || c == "white") return true;
	}
      }
    }  
  }else {
    for (int i = 19; i < 25; i++) {
      if (b->color_on_space(i) != "\033[0;42;30mblack\033[1;42;37m") continue;
      for (int j=0;j<2;j++){
	if (!curr_taken[j]){
	  if (i + dice[j] >= 25) return true;	  
	  string c =  b->color_on_space(i + dice[j]);
	  if (c=="empty" || c == "\033[0;42;30mblack\033[1;42;37m") return true;
	}
      }
    }      
  }
  return false;*/
//}

  
bool Game::check_move(bool print,int start, int end){
  if (test_in_zone(turn)) {
    return check_zone_move(print,start,end);
  }

  if (start < 0 || start > 25) {
    if (print)  cout << "coordinate is out of bounds. please try again." << endl;
    return false;
  }
  
  else if ((b->get_knocked("black") == 0 && start == 0) || (b->get_knocked("white") == 0 && start == 25)){	
    if (print) cout << "faulty starting coordinates. black on bar starts at 0; white on bar starts at 25. please try again." << endl;
    return false;
  }    
  if (start > 0 && start < 25){
    if(b->test_space_empty(start)){
      if (print) cout << "board is empty at that starting position. please try again." << endl;      
      return false;
    }
    if(((turn == "black") && (b->color_on_space(start) == "white")) ||
       ((turn == "white") && (b->color_on_space(start) == "\033[0;42;30mblack\033[1;42;37m"))) {
      if (print) cout << "wrong starting color at that location. please try again." << endl;
      return false;
    }
  }
  if (start == end) {
    if (print) cout << "start and end are same position" << endl;
    return false;
  }
  if (!test_in_zone(turn) && (end > 24 || end < 1)){
    if (print) cout << "end position out of bounds. please try again." << endl;
    return false;
  }  
  if (((start > end) && (turn == "black")) ||
      ((start < end) && (turn == "white"))) {
    if (print) cout << "wrong direction. please try again." << endl;
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

  if (((start < end) &&
       ((end - start != dice[0]) &&
	(end - start != dice[1]))) ||
      ((start > end) &&
       ((start - end != dice[0]) &&
	(start - end != dice[1]))))
    {    
      if (print) cout << "incorrect move given your dice roll. please try again" << endl;
      return false;
    } 
  if (end > 0 && end < 25){
    if((((turn == "black") && (b->color_on_space(end) == "white")) ||
	((turn == "white") && (b->color_on_space(end) == "\033[0;42;30mblack\033[1;42;37m")))){
      if (b->num_pips_on_space(end) > 1) {
	if (print) cout << "cannot move pip there; location is unhitable. please try again." << endl;
	return false;
      }
    }
  }
  
  if ((turn == "black" && b->get_knocked("black") > 0 && start != 0) ||
      (turn == "white" && b->get_knocked("white") > 0 && start != 25)) {
    if (print) cout << "you must remove your piece(s) from the bar before continuing." << endl;
    return false;
  }
  return true;
}

bool Game::determine_valid_moves(string color)
{
  if (color == "white")
    {
      if (test_in_zone(turn)){
	for (int i = 6; i > 0; i--) {
	  if (check_zone_move(false, i, i - dice[0])) return true;
	  if (check_zone_move(false, i, i - dice[1])) return true;	  
	}
      }else {
	for (int i = 25; i > 0; i--){
	  if (!curr_taken[0]){
	    if (check_move(false, i, i - dice[0])) return true;
	  }
	  if (!curr_taken[1]){
	    if (check_move(false, i, i - dice[1])) return true;
	  }	     
	}
      }
      return false;
    }
  else
    {
      if (test_in_zone(turn)){
	for (int i = 19; i < 25; i++) {
	  if (check_zone_move(false, i, i + dice[0])) return true;
	  if (check_zone_move(false, i, i + dice[1])) return true;	  
	}
      }else{
	for (int i = 0; i < 25; i++){
	  if (!curr_taken[0]){
	    if (check_move(false, i,i + dice[0])) return true;
	  }
	  if (!curr_taken[1]){
	    if (check_move(false, i, i + dice[1])) return true;
	  }      
	}
      }
      return false;
    }
}

void Game::make_move()
{
  if (first_pos == 0) { b->remove_knocked("\033[0;42;30mblack\033[1;42;37m"); }
  if (first_pos == 25)  { b->remove_knocked("white"); }

  if (second_pos > 0 && second_pos < 25){
    //deal with a knock
    if (b->hitable(second_pos)){
      if ((turn == "white" && b->color_on_space(second_pos) != "white") ||
	  (turn != "white" && b->color_on_space(second_pos) == "white")){
	//if (second_pos < 25 && second_o > 0)
	b->remove_one_from_space(second_pos);
	cout << "knocked!" << endl;
	if (turn == "white") b->add_knocked("black");
	else                 b->add_knocked("white");
      }
    }
  }
  if (first_pos != 25 && first_pos != 0)
    b->remove_one_from_space(first_pos);
  if (second_pos < 25 && second_pos > 0){
    if (turn == "white")
      b->add_one_to_space(second_pos,turn);
    else
      b->add_one_to_space(second_pos,"\033[0;42;30mblack\033[1;42;37m");
  }else {
    if (turn == "white")
      b->add_one_off_board("white");
    else
      b->add_one_off_board("black");
  }  
}

bool Game::validate_input(){
  std::string::const_iterator it = strin1.begin();
  while (it != strin1.end() && std::isdigit(*it)) ++it;
  if (!(!strin1.empty() && it == strin1.end())) return false;

  it = strin2.begin();
  while (it != strin2.end() && std::isdigit(*it)) ++it;
  return (!strin2.empty() && it == strin2.end());
  }

void Game::get_move(){
  cin >> strin1 >> strin2; cin.get();//first_pos >> second_pos; cin.get();
  while(!validate_input()){
    cout << "not valid numerical input. please try again." << endl;
    cin >> strin1 >> strin2; cin.get();
   }
  first_pos = stoi(strin1);
  second_pos = stoi(strin2);
  cout << "FIRST: " << first_pos << " SECOND: " << second_pos << endl;
}

void Game::take_turn(){
  if (!first_move) {
    dice = b->roll_dice();
    cout << "roll is: " << dice[0] << " " << dice[1] << endl;
  } else first_move = false;
  
  curr_taken[0] = false;
  curr_taken[1] = false;
  
  int stopval = 2;   
  if (dice[0] == dice[1]){
    cout << "DOUBLE! player has 4 moves" << endl;
    stopval = 4;
  }
  
  if (!determine_valid_moves(turn)){
    cout << "player has no valid moves." << endl;
    stopval = 0;
  }
  
  for (int i=0; i < stopval; i++){
    if (stopval == 4 && i == 2) {
      curr_taken[0] = false;
      curr_taken[1] = false;
    }
    if (!determine_valid_moves(turn)){
      cout << "player has no valid moves." << endl;
      break;
    }
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

void Game::determine_first_turn(){   
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


int main(){
  Game *g = new Game();
  g->run_game();
  return 0;
}
