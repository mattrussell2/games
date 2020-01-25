#include "Game.h"
#include "Board.h"

//issues: when in zone:  able to take a piece off the board when difference is < dice roll even if there are
//        pieces above that piece that could be moved (on the board)
// when have doubles, often valid moves not being correctly determined. sometimes no valid moves. 

void Game::run_game()
{
  b = new Board();
  b->set_score(white_score, black_score);
  b->print_board();
  determine_first_turn();
  double_declined = false;
  double_turn = "empty";
  first_move = true;
  string s;
  while(!check_gameover())
    {
      take_turn();
      if (check_gameover()) break;
      if (double_turn == "empty" or double_turn == turn){
        cout << turn << "\'s turn! press <enter> to roll, or d to offer to double. ";
        getline(cin,s,'\n');
        if (s[0] == 'd') double_cube();
      }
      else {
        cout << turn << "\'s turn! press <enter> to roll.";
        getline(cin,s,'\n');
      }
    }
  if (b->white_win())  end_game("white");
  else                 end_game("black");
  b->set_score(white_score, black_score);
  cout << "thanks for playing." << endl;
}

void Game::end_game(string winner){
  string loser;
  bool in_home_test;
  if (winner == "white") {
    loser = "black";
    in_home_test = b->any_black_in_white_home();
  }
  else {
    loser = "white";
    in_home_test = b->any_white_in_black_home();
  }
  int score_to_add = 0;
  cout << winner << " wins!" << endl;
  if (!double_declined){
    int num_off = b->num_off(loser);
    bool gammon = num_off == 0;
    bool backgammon = (gammon and (b->num_knocked(loser) > 0 or in_home_test));
    if (backgammon){
      cout << "it's a backgammon!" << endl;
      score_to_add = b->double_amount() * 3;
    }else if (gammon){
      cout << "it's a gammon!" << endl;
      score_to_add = b->double_amount() * 2;
    }else {
      score_to_add = b->double_amount();
    }
  }else{
    score_to_add = b->double_amount();
  }
  if (winner == "white") white_score += score_to_add;
  else                   black_score += score_to_add;
}

void Game::welcome_message()
{  
  cout << "welcome to backgammon!" << endl;
  cout << "moves are made one by one. " << endl;
  cout << "format: firstpos <space> secondpos <enter>. " << endl;
  cout << "use the 0 and 25 positions for the bar/off board." << endl;
  cout << "to double, enter d instead of <enter> to roll dice" << endl;
  cout << "press <enter> to auto-roll for first move." << endl;
  cin.get();
}

bool Game::test_in_zone(){
  if (b->get_knocked(turn) > 0) return false;
  int curr, end;
  if (turn == "white") { curr = 7; end = 24; }
  else                 { curr = 1; end = 18; }
  while (curr != end) {
    if (b->color_on_space(curr) == turn) return false;
    curr++;
  }
  return true; 
}

//we know all pieces are in the zone. if we have a move within the board, we have 
//to take it
bool Game::check_zone_move(int start, int end, bool print){  
  int difference;
  if (turn == "white") difference = start - end;
  else                 difference = end - start;
  
  if ((turn == "white" and (start > 7 or end > 7 or start < 0 or end < 0)) or
      (turn == "black" and (start < 19 or end < 19 or start > 25 or end > 25)))
    {
      if (print) cout << "coordinates out of bounds. please try again." << endl;
      return false;
    }

  if (((difference > dice[0] and difference > dice[1]) or
       (curr_taken[0] and difference > dice[1]) or
       (curr_taken[1] and difference > dice[0]))){
    if (print) cout << "move is too large. please try again." << endl;
    return false;
  }  

  if ((end == 0 or end == 25) and
      ((!curr_taken[0] and difference == dice[0]) or
       (!curr_taken[1] and difference == dice[1]))) return true;

  if (end > 0 and end < 25 and difference != dice[0] and difference != dice[1]){
    if (print) cout << "incorrect move. please try again\n";
    return false;
  }
  
  if (end > 0 and end < 25){
    if ((!curr_taken[0] and difference == dice[0]) or
        (!curr_taken[1] and difference == dice[1])){
      if (turn != b->color_on_space(end) and b->num_pips_on_space(end) > 1){
        if (print) cout << "cannot go here." << endl;
        return false;
      }
    }
  }
  
  //ending on the bar
  //if die roll is greater than the difference, make sure there aren't any higher possibilities.
  if (end == 0 or end == 25){
    int curr; 
    if (end == 0) curr = 6;
    else          curr = 19;
    while(curr != start){
      if (b->num_pips_on_space(curr) > 0 and b->color_on_space(curr) == turn){
        if (((turn == "white") and
             (check_zone_move(curr, curr - dice[0]) or
              check_zone_move(curr, curr - dice[1]))) or
            ((turn == "black") and
             (check_zone_move(curr, curr + dice[0]) or
              check_zone_move(curr, curr + dice[1])))){
          if (print) cout << "a higher piece can be moved first" << endl;
          return false; 
        }
      }
      if (turn == "white") curr--;
      else                 curr++;
    }

  }
  return true; 	   
}
  
bool Game::check_move(int start, int end, bool print){
  if (start < 0 or start > 25) {
    if (print) cout << "coordinate out of bounds. please try again." << endl;
    return false;
  }
 
  if (start == end) {
    if (print) cout << "start and end are same position" << endl;
    return false;
  }

  if ((turn == "white" and b->get_knocked("white") == 0 and start == 0) or
      (turn == "black" and b->get_knocked("black") == 0 and start == 25)){
    if (print) cout << "cannot start on bar unless there are knocked pips\n";
    return false;
  }

  if ((turn == "black" and b->get_knocked("black") > 0 and start != 0) or
      (turn == "white" and b->get_knocked("white") > 0 and start != 25)) {
    if (print) cout << "remove your piece(s) from the bar before continuing.\n";
    return false;
  }
  
  if (((start > end) and (turn == "black")) or
      ((start < end) and (turn == "white"))) {
    if (print) cout << "wrong direction. please try again." << endl;
    return false;
  }

  if (dice[0] != dice[1] and
      ((abs(start - end) == dice[0] and curr_taken[0]) or
       (abs(start - end) == dice[1] and curr_taken[1]))){
    if (print)
      cout << "already moved that die. please try again.\n";
    return false;
  }
  
  if (test_in_zone()) return check_zone_move(start, end, print);

  if (end == 25 or end == 0) {
    if (print) cout << "cannot take pieces off board yet!" << endl;
    return false;
  }
  int difference; 
  if (turn == "black") difference = end - start;
  else                 difference = start - end;
  if (difference != dice[0] and difference != dice[1]) {    
    if (print) cout << "incorrect move. please try again\n" << endl;
    return false;
  }
    
  if (start > 0 and start < 25 and b->color_on_space(start) != turn){
    if (print)
      cout << "wrong starting color at that location. please try again.\n";
    return false;
  }            
        
  if (start > 0 and start < 25 and b->test_space_empty(start)) {
    if (print)
      cout << "board is empty at that starting position. please try again.\n";
    return false;
  }
   
  if (end > 0 and end < 25 and b->num_pips_on_space(end) > 1 and
      b->color_on_space(end) != turn){
      if (print)
        cout << "cannot move pip there. please try again.\n";
      return false; 
  }
  
  return true;
}

bool Game::determine_valid_moves(string color)
{
  for (int i = 0; i < 26; i++){
    if ((color == "white" and (check_move(i, i - dice[0]) or
                               check_move(i, i - dice[1]))) or
        (color == "black" and (check_move(i, i + dice[0]) or
                               check_move(i, i + dice[1]))))
      return true;
  }
  return false;
}

void Game::make_move()
{
  if (first_pos == 0)  b->remove_knocked("black");
  if (first_pos == 25) b->remove_knocked("white");

  if (second_pos > 0 and second_pos < 25){
    //deal with a knock
    if (b->hitable(second_pos) and turn != b->color_on_space(second_pos)){
      b->remove_one_from_space(second_pos);
      cout << "knocked!" << endl;
      if (turn == "white") b->add_knocked("black");
      else                 b->add_knocked("white"); 
    }
  }
  if (first_pos != 25 and first_pos != 0) b->remove_one_from_space(first_pos);
  if (second_pos < 25 and second_pos > 0) b->add_one_to_space(second_pos, turn);
  else                                    b->add_one_off_board(turn); 
}

bool Game::validate_input(){ 
  std::string::const_iterator it = strin1.begin();
  while (it != strin1.end() and std::isdigit(*it)) ++it;
  if (!(!strin1.empty() and it == strin1.end())) return false;

  it = strin2.begin();
  while (it != strin2.end() and std::isdigit(*it)) ++it;
  return (!strin2.empty() and it == strin2.end());
}

bool Game::double_cube(){
  string opponent, accept;
  if (turn == "white") opponent = "black"; else opponent = "white";
  cout << turn << " has offered a double. " << opponent
       << ", do you accept? (y/n) " << endl;
  getline(cin, accept); 
  while (accept != "y" and accept != "n") {
    cout << "please enter y/n" << endl;
    getline(cin, accept); 
  }
  if (accept == "y"){
    b->set_double(b->double_amount() * 2);
    if (turn == "white") double_turn = "black";
    else                 double_turn = "white";    
    cout << "double accepted." << endl;
    cout << turn << " to move" << endl;    
    return true;
  }
  else {
    if (turn == "white") b->decline_double("black");
    else                 b->decline_double("white");
    double_declined = true;
    return false;
  }
}

bool Game::get_move(){
  cin >> strin1 >> strin2; getchar();
  while(!validate_input()){
    cout << "not valid numerical input. please try again." << endl;
    cin >> strin1 >> strin2; getchar();
  }
  first_pos = stoi(strin1);
  second_pos = stoi(strin2);
  return true;  
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

  bool endtest;
  for (int i = 0; i < stopval; i++){
    if (check_gameover()) return;
    if (stopval == 4 and i == 2) {
      curr_taken[0] = false;
      curr_taken[1] = false;
    }
    if (!determine_valid_moves(turn)){      
      cout << "player has no valid moves." << endl;
      break;
    }
    endtest = get_move();
    if (!endtest) return;
    while(!check_move(first_pos, second_pos, true)){
      endtest = get_move();	
    }
    if (!endtest) return;
    make_move();
    if (((first_pos < second_pos) and (second_pos - first_pos) == dice[0]) or
        ((first_pos > second_pos) and (first_pos - second_pos) == dice[0])) {
      curr_taken[0] = true;
    } else curr_taken[1] = true;

    b->print_board();
    if (i < stopval - 1)
      cout << "move made. [dice read: " << dice[0] << " " << dice[1] << "]\n";
  }        
  if (turn == "white") turn = "black";
  else                 turn = "white";   
}

void Game::determine_first_turn(){   
  dice = b->roll_dice(); 
  while (dice[0] == dice[1]){
    cout << "white rolls: " << dice[0] << endl;
    cout << "black rolls: " << dice[1] << endl;
    cout << "It's a tie! Press <enter> to reroll" << endl;
    cin.get();
    dice = b->roll_dice();
  }
  cout << "white rolls " << dice[0] << endl;
  cout << "black rolls " << dice[1] << endl;
  if (dice[0] > dice[1]){
    cout << "white goes first!" << endl;
    turn = "white";
  }else  {
    cout << "black goes first!" << endl;
    turn = "black";
  }
  
  cout << "[dice read: " << dice[0] << " " << dice[1] << "]" << endl;
};


int main(){
  Game *g = new Game();
  g->welcome_message();

  string yn;
  while(true){
    cout << endl; cout << endl;
    cout << "new game!" << endl;
    g->run_game();
    cout << "the score is --- " << "white: " << g->white_score << " black: "
         << g->black_score << endl;
    cout << "would you like to keep playing? (y/n)" << endl;
    getline(cin, yn); 
    while (yn[0] != 'y' and yn[0] != 'n'){
      cout << "please enter y or n" << endl;
      getline(cin,yn);
    }
    if (yn[0] == 'n') break;
  }
  if (g->white_score > g->black_score) cout << "white wins!" << endl;
  else if (g->black_score > g->white_score) cout << "black wins!" << endl;
  else cout << "it's a tie!!!" << endl;
  cout << "\nThanks for playing :)" << endl;  				   
  return 0;
}
