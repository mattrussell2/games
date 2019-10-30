//battleship.cpp
//by matt russell
//2019

#include "battleship.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>
#include <math.h>


void battleship::start_game(){
  std::cout << R"(
___.              __     __   .__                   .__     .__         
\_ |__  _____   _/  |_ _/  |_ |  |    ____    ______|  |__  |__|______
 | __ \ \__  \  \   __\\   __\|  |  _/ __ \  /  ___/|  |  \ |  |\____ \
 | \_\ \ / __ \_ |  |   |  |  |  |__\  ___/  \___ \ |   Y  \|  ||  |_> >
 |___  /(____  / |__|   |__|  |____/ \___  >/____  >|___|  /|__||   __/  
     \/      \/                          \/      \/      \/     |__|     
	    )" << std::endl;

  std::cout << "Welcome to battleship!" << std::endl;
  std::cout << "You have the option to place the ships on the board, or have "
	    << std::endl;
  std::cout << "me do it for you." << std::endl;

  string s;
  std::cout <<
    "Would you like to place the ships on the board yourself (y/n)? ";
  getline(cin,s);
  while (s!="y" && s!="n" && s!="Y" && s!="N") { 
    std::cout << "please enter y/n: ";
    getline(cin,s);
  }    
  std::cout << std::endl;

  srand(time(NULL));
  if (s=="y" || s=="Y") {
    generate_ships(hu_own_board,hu_ships,true);
  } else{
    generate_ships(hu_own_board,hu_ships,false);  
  }
  generate_ships(pc_own_board,pc_ships,false);  
 
  std::cout << "Okay! How difficult would you like me?" << std::endl;
  std::cout << "(0 = easy, 1 = medium, 2 = hard): " << std::endl;
  getline(cin,s);
  while(s!="0" && s!="1" && s!="2"){
    std::cout << "Enter 0 or 1 or 2: ";
    getline(cin,s);
  }
  if (s == "0") difficulty = "easy";
  if (s == "1") difficulty = "medium";
  if (s == "2") difficulty = "hard";
  std::cout << std::endl;
  print_user_game_data();
  std::cout << std::endl;
}

battleship::battleship(){  
  pc_pboard = new pboard();      hu_pboard = new pboard();   //probabilities
  pc_try_board = new board();    hu_try_board = new board(); //attempts
  pc_own_board = new board();    hu_own_board = new board(); //ships

  init_board(pc_try_board);      init_board(hu_try_board);   
  init_board(pc_own_board);      init_board(hu_own_board);  
  init_pboard(pc_pboard);        init_pboard(hu_pboard);

  pc_ships = new vector<ship>(); hu_ships = new vector<ship>();
}

battleship::~battleship(){
  delete pc_pboard;        delete hu_pboard;
  delete pc_own_board;     delete hu_own_board;
  delete pc_try_board;     delete hu_try_board;  
  delete pc_ships;         delete hu_ships;
}

//initialize a 10x10 probability board 
void battleship::init_pboard(pboard *pb){
  vector<int> a;
  for (int i = 0; i < 10; i++){
    pb->push_back(a);
    for (int j = 0; j < 10; j++){
      pb->at(i).push_back(0);
    }
  }
}

//initialize a board of ships or attempt board
void battleship::init_board(board *b){
  vector<string> v;
  for (int i = 0; i < 10; i++){
    b->push_back(v);
    for (int j = 0; j < 10; j++){
      b->at(i).push_back("\033[1;34m*\033[0m"); 
    }
  }
} 

//given a board and a location, test if there's a hit ship there
bool battleship::test_for_hit(board *b, int i, int j) const{
  return (b->at(i).at(j)!="\033[1;34m*\033[0m" && 
	  b->at(i).at(j)!="\033[1;37m_\033[0m" &&
	  b->at(i).at(j)!="\033[1;31mX\033[0m");
}

//test for end of game (if there are 17 hits, gg)
bool battleship::gameover() const{
  int pc_count = 0;
  int usr_count = 0;
  for (int i = 0; i < 10; i++){    
    for (int j = 0; j < 10; j++){
      if (test_for_hit(pc_try_board,i,j)) pc_count++;
      if (test_for_hit(hu_try_board,i,j)) usr_count++;
    }
  }
  if (pc_count == 17){    
    print_full_game_data("I WIN. BWAHAHAHAHAA!!!!");
    return true;
  }
  if (usr_count == 17){   
    print_full_game_data("YOU WIN. NOOOOOOOOOOOO!!!");
    return true;
  }
  return false;
}

//adds the hit to the vector of ships; if a ship is sunk, display info
void battleship::add_hit_to_ships(board *b,vector<ship> *ships, pboard *pb,
				  int x, int y, bool disp){
  int q = (int)ships->size();
  for (int i = 0; i < q; i++){
    if (ships->at(i).within_ship(x,y)){
      ships->at(i).add_hit_to_ship(x,y);
      if (ships->at(i).is_sunk()) {
	ship s = ships->at(i);
	if (disp){	  
	  std::cout << "ARGH! YOUVE SUNK ME SHIP: ";
	  string c = s.get_char();
	  if (c == "\u001b[33;1mA\u001b[0m")
	    std::cout << "DRATS! An Aircraft Carrier!" << std::endl;
	  if (c == "\u001b[36;1mB\u001b[0m")
	    std::cout << "OH NO! A Battleship!" << std::endl;
	  if (c == "\u001b[37;1mC\u001b[0m")
	    std::cout << "EGADS! A Cruiser!" << std::endl;
	  if (c == "\u001b[35;1mS\u001b[0m")
	    std::cout << "GOODNESS! A Submarine!" << std::endl;
	  if (c == "\u001b[32;1mD\u001b[0m")
	    std::cout << "RATS! A Destroyer" << std::endl;       
	}
	string sunk_string = "\033[1;31m" + s.get_name().substr(0,1) +
	                     "\033[0m";
	//replace the ship on the try board with the char of the ship
	place_ship(b, s.get_length(), s.get_x(), s.get_y(),
		   s.get_dir(), sunk_string); 
	pb->at(s.get_x()).at(s.get_y()) = -2; //sunk probability
	return;
      }
    }    
  }
}

void battleship::register_hit(board *hit_board, board *firing_board,
			      pboard *f_pboard, int x, int y){
  hit_board->at(y).at(x) = "\033[1;31mX\033[0m";
  firing_board->at(y).at(x) = "\033[1;31mX\033[0m"; 
  f_pboard->at(y).at(x) = -1;
}

void battleship::register_miss(board *missed_board, board *firing_board,
			       pboard *f_pboard, int x, int y){
  missed_board->at(y).at(x)="\033[1;37m_\033[0m";
  firing_board->at(y).at(x)="\033[1;37m_\033[0m";
  f_pboard->at(y).at(x) = -3;
}

string battleship::hu_make_guess(){
  int x, y;
  string input, retstr;
  std::cout << "Enter a coordinate (i.e. A3): ";
  getline(cin,input);
  y = int(toupper(input[0])) - 65;
  if (input.length() > 2)
    x = ((int)(toupper(input[1])) - 48) * 10 + (int)(toupper(input[2])) - 48;
  else
    x = (int)(toupper(input[1])) - 48;
  
  if (x > 9 || y > 9 || x < 0 || y < 0){
    return "Please enter valid coordinates.";
  }
  else{
    retstr = "You guess:  \u001b[32;1m" + input + "\u001b[0m - ";
    if (pc_own_board->at(y).at(x) != "\033[1;34m*\033[0m") {//"*") {
      if (pc_own_board->at(y).at(x) == "\033[1;31mX\033[0m"){
	return "Already hit there!";
      }
      if (pc_own_board->at(y).at(x) == "\033[1;37m_\033[0m"){
	return "Already missed there!";
      }else{ 	
	register_hit(pc_own_board, hu_try_board, hu_pboard, x, y);
	retstr += "\033[1;31mhit!\033[0m";
	hu_guesses.push_back("\u001b[31;1m" + input + "\u001b[0m");
	add_hit_to_ships(hu_try_board, pc_ships, hu_pboard, x, y, 1);           
      }
    }else {      
      register_miss(pc_own_board, hu_try_board, hu_pboard, x, y);
      retstr += "\033[1;37mmiss!\033[0m";
      hu_guesses.push_back("\u001b[37;1m" + input + "\u001b[0m");
    }
  }
  return retstr;
}

void battleship::easy_guess(board *b, int &x, int &y) const{
  srand(time(NULL));
  x = rand() % 10;
  y = rand() % 10;
  while (b->at(y).at(x) != "\033[1;34m*\033[0m"){
    x = rand() % 10;
    y = rand() % 10;
  }
}

// look for hit ships.
// if one is found, x,y will be updated accordingly, and return true. 
bool battleship::search_for_hit_location(board *b, int &x, int &y) const{
  int converted = y * 10 + x;
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if (i * 10 + j < converted) continue;
      if (b->at(i).at(j) == "\033[1;31mX\033[0m"){
	y = i;
	x = j;
	return true;
      }
    }
  }
  return false;
}

//checks to see if a given location is valid to fire;
bool battleship::is_valid_loc(board *b, int x, int y) const{
  if (x > 9 || y > 9 || x < 0 || y < 0) return false;
  if (b->at(y).at(x) == "\033[1;34m*\033[0m") return true;
  else return false;      
}


//pre: a ship has already been hit. 
//post: x and y are set to a new location to aim for.
bool battleship::blood_in_the_water(board *b,int &x, int &y) const{
  if (is_valid_loc(b, x + 1, y)) { x++; return true; }
  if (is_valid_loc(b, x - 1, y)) { x--; return true; }
  if (is_valid_loc(b, x, y + 1)) { y++; return true; }
  if (is_valid_loc(b, x, y - 1)) { y--; return true; }
  return false; //this location may not be viable (another spot on ship is...)
}
      
//finds an okay spot, given that a ship has been hit already
void battleship::find_medium_good_spot(board *b, int &x, int &y) const{
  // loop until finding an x to aim around
  bool valid=false;
  while (!valid){
    search_for_hit_location(b, x, y); //now x and y are valid coordinate of hit.
    valid = blood_in_the_water(pc_try_board, x, y); //if it works, we're done.
    if (!valid){ //have to increment the position for next call to search
      x++;
      if (x > 9) {
	x = 0;
	y++;
      }
    }
  }  
}

//checks if a given area is clear of ships
bool battleship::check_nearby_hit(board *board, int len, int x,
				  int y, int d) const{
 if (d == 0){
    for (int z = x; z < x + len; z++){
      if (board->at(y).at(z) == "\033[1;31mX\033[0m") return true;
    }
  }
  else if (d == 1){
    for (int z = y; z< y + len; z++){
      if (board->at(z).at(x) == "\033[1;31mX\033[0m") return true;
    }
  }
  return false;
}

void battleship::update_probabilities(board *b,pboard *pb, int x,
				      int y, int len, bool d) {
  int modifier, boundcheck;
  if (d == 0) {
    boundcheck = x + len - 1;
  }
  else {
    boundcheck = y + len - 1;
  }
  if (boundcheck <= 9 && check_clear_area(b, len, x, y, d)){
      if (check_nearby_hit(b, len, x, y, d)){
	modifier = 50;
      }
      else {
	modifier = 1;
      }
      for (int i = 0; i < len; i++){
	if (d == 0)
	  pb->at(y).at(x + i) += modifier;
	else
	  pb->at(y + i).at(x) += modifier;
      }
    }
}
     
void battleship::calc_probabilities(board *b, pboard *pb){  
  int len;
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){ //for each space
      for (int z = 0; z < 5; z++){ //for each ship
	len = ship_len_lookup[z];
	update_probabilities(b, pb, j, i, len, 0);
	update_probabilities(b, pb, j, i, len, 1);
      }
    }
  }
}

struct p{
  int x;
  int y;
};

void battleship::find_best_guess(board *b, pboard *pb, int &x, int &y){
  int largest_p = 0;
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if (pb->at(i).at(j) > largest_p && is_valid_loc(b, j, i)){
	largest_p = pb->at(i).at(j);
	x = j;
	y = i;
      }
    }
  }
  vector<p> possibles;
  p a;
  //first set of 
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++) {
      if (pb->at(i).at(j) == largest_p && is_valid_loc(b, j, i)){
	a.x = j;
	a.y = i;
	possibles.push_back(a);
      }
    }
  }
  int ran = possibles.size();
  if (ran == 1) {
    ran = 0;
  }
  else {
    ran = rand() % (ran - 1);
  }
  x = possibles.at(ran).x;
  y = possibles.at(ran).y;
}

string battleship::pc_make_guess(){
  int x; int y; bool found;
  string retstr = "";
  if (difficulty == "easy") easy_guess(pc_try_board, x, y);
  if (difficulty == "medium"){
    x = 0;
    y = 0;
    found = search_for_hit_location(pc_try_board, x, y);
    if (!found){
      easy_guess(pc_try_board, x, y);  
    }else{
      find_medium_good_spot(pc_try_board, x, y);
    }
  }
  if (difficulty == "hard") {
    delete pc_pboard;
    pc_pboard = new pboard();
    init_pboard(pc_pboard);
    
    calc_probabilities(pc_try_board, pc_pboard);
    find_best_guess(pc_try_board, pc_pboard, x, y);
  }
  
  retstr = "PC guesses: \u001b[32;1m" + string(1,(char)(y+65)) +
            to_string(x) + "\u001b[0m - ";
  if (hu_own_board->at(y).at(x) != "\033[1;34m*\033[0m") {
    retstr += "\033[1;31mhit!\033[0m";
    pc_guesses.push_back("\u001b[31;1m" + string(1,(char)(y+65)) +
			 to_string(x) + "\u001b[0m");
    register_hit(hu_own_board, pc_try_board, pc_pboard, x, y); 
    add_hit_to_ships(hu_own_board, hu_ships, pc_pboard, x, y, 1); 
    add_hit_to_ships(pc_try_board, hu_ships, pc_pboard, x, y, 0);
  }else {
    retstr += "\033[1;37mmiss!\033[0m";
    pc_guesses.push_back("\u001b[37;1m" + string(1,(char)(y+65)) +
			 to_string(x) + "\u001b[0m");
    register_miss(hu_own_board, pc_try_board, pc_pboard, x, y);
  }
  return retstr;
}


//continue allowing the user to guess until one player wins
void battleship::run_game(){ 
  int turn_counter=0;
  start_game();
  string hu_guess, pc_guess;
  while(!gameover()){
    hu_guess = hu_make_guess();
    if (hu_guess == "already hit here!" ||
	hu_guess == "already missed here!" ||
	hu_guess == "Please enter valid coordinates."){
      std::cout << hu_guess << std::endl << "Please guess again." << std::endl;
      continue;
    }
    pc_guess = pc_make_guess();
    print_user_game_data();    
    std::cout << std::endl;
    turn_counter++;
  }
  std::cout << "The game was finished in " << turn_counter << " turns." <<
    std::endl;
}


//checks if a given area is clear of ships
bool battleship::check_clear_area(board *board, int len,
				  int x, int y, int d) const{
  int count = 0;
  if (d == 0){
    for (int z=x; z<x+len; z++){
      if (board->at(y).at(z) == "\033[1;34m*\033[0m" ||
	  board->at(y).at(z) == "\033[1;31mX\033[0m")
	count++;
    } 
  }
  else if (d == 1){
    for (int z=y; z<y+len; z++){
      if (board->at(z).at(x) == "\033[1;34m*\033[0m" ||
	  board->at(z).at(x) == "\033[1;31mX\033[0m")
	count++;
    }
  }
  if (count == len) return true;
  else return false;
}

// i is ship length; x is x loc, y is y loc,
// d is direction (1 vertical, 0 horizontal)
// returns when it finds a valid placement position for the given ship
void battleship::find_valid_start_loc(board *board, int len,
				      int &x, int &y, bool &d) const{  
  while (true){
    x = (rand() % 10);
    y = (rand() % 10);
    d = rand() % 2; //0 or 1, indicating direction  
    if ((d==0 && (x+len<=10)) ||  (d==1 && (y+len<=10))){
      if (check_clear_area(board, len, x, y, d)){
	break;
      }
    }
  }			   
}

//put a ship in a board
void battleship::place_ship(board *b, int len, int x, int y, bool d, string s){
  if (d == 0) {
    for (int z=x; z<x+len; z++){
      b->at(y).at(z) = s;
    }
  }
  if (d == 1) {
    for (int z=y; z<y+len; z++){
      b->at(z).at(x) = s;
    }    
  }
}


bool battleship::game_ready(board *human_board) const {
  int usr_star_count = 0;
  for (int i=0; i<10; i++){
    for (int j=0; j<10; j++){
      if (human_board->at(i).at(j) == "\033[1;34m*\033[0m")
	usr_star_count++;
    }
  }
  return (usr_star_count == 66);
}

bool toBool(string s);
bool toBool(string s){
  return (s != "0");
}

//gets starting coordinate from human
void battleship::get_start_coord(board *b, int len,
				 int &x, int &y, bool &d) const{
  bool badinput=true;
  string coord;
  while(badinput){
    std::cout << "Enter a starting coordinate (i.e. J5): ";
    
    getline(cin,coord);
    y = int(coord[0]) - 65;
    if (coord.length() > 2){
      x = ((int)coord[1]-48)*10 + (int)coord[2]-48;
    }else x = (int)coord[1] - 48;
    if (x < 0 || x > 9 || y < 0 || y > 9) {
      std::cout << "Bad coordinate. Please try again." << std::endl;
      continue;
    }
    else badinput=false;
  }
  std::cout << "Enter a direction (0 for horizontal, 1 for vertical): "; 
  getline(cin,coord);
  d = toBool(coord);
  std::cout << "D: " << d << std::endl;
  if ((d==0 && (x+len-1>9)) ||  (d==1 && (y+len-1>9))){
    std::cout << "Ship will fall off the board. Please try again." << std::endl;
    get_start_coord(b, len, x, y, d);
  }
    
  if (!check_clear_area(b, len, x, y, d)){
    std::cout << "Already a ship there! Please try again." << std::endl;
    get_start_coord(b, len, x, y, d);
  }
}

void battleship::generate_ships(board *b, vector<ship> *ships, bool hu_pc){
  int x, y, len;
  bool direction;
  string ship_name;
  string ship_class;
  ship s;
  for (int i = 0; i < 5; i++){
    len = ship_len_lookup[i];
    ship_class = ship_class_lookup[i];
    ship_name = ship_name_lookup[i];
    if (hu_pc){
      std::cout << "Placing ships of type " << ship_name << " (length: " <<
	len << ")" << std::endl;
      print_board(b);
      get_start_coord(b, len, x, y, direction);      
    }else {
      find_valid_start_loc(b, len, x, y, direction);
    }
    place_ship(b, len, x, y, direction, ship_class);
    s = ship(ship_name, ship_class, x, y, len, direction);
    s.set_hits();
    ships->push_back(s);
  }
}

void print_digits();
void print_digits(){
  std::cout << "   \033[1;32m0123456789\033[0m";
  std::cout << "               \033[1;32m0123456789\033[0m\n";
}

void print_header();
void print_header(){
  std::cout << "   YOUR SHOTS               YOUR SHIPS       YOUR MOVES";
  std::cout << "    PC MOVES" << std::endl;
}

string green_char(int i);
string green_char(int i){
  return "\033[1;32m" + string(1,char(65+i)) + "\033[0m";
}

void battleship::print_board(board *b) const{
  std::cout << "   Your board" << std::endl;
  std::cout << "   \033[1;32m12345678910\033[0m" << std::endl;
  for (int i=0; i<10; i++){
    std::cout << " " << char(65+i) << " ";
    for (int j=0; j<10; j++){
      std::cout << b->at(i).at(j);
    }
    if (i < (int)hu_guesses.size() && i < (int)pc_guesses.size()){
      std::cout << " " << green_char(i) << "         " <<
	hu_guesses[hu_guesses.size()-i-1] << "           " <<
	pc_guesses[pc_guesses.size()-i-1] << std::endl;
    }else{
      std::cout << " " << green_char(i) << std::endl;
    }	  
  }    
}

void battleship::print_two_boards(board *b1, board*b2) const{
  print_digits();
  for (int i=0; i<10; i++){
    std::cout << " " << green_char(i) << " ";
    for (int j=0;j<10;j++){
      std::cout << b1->at(i).at(j);
    }
    std::cout << " " << green_char(i) << "           " << green_char(i) << " ";
    
    for (int j=0; j<10; j++){
      std::cout <<  b2->at(i).at(j);
    }
    if (i < (int)hu_guesses.size() && i < (int)pc_guesses.size()){
      std::cout << " " << green_char(i) << "         " <<
	hu_guesses[hu_guesses.size()-1-i] << "           " <<
	pc_guesses[pc_guesses.size()-1-i] << std::endl;
    }else{
      std::cout << " " << green_char(i) << std::endl;
    }
  }
  print_digits();
}

void battleship::print_user_game_data() const{
  print_header();
    print_two_boards(hu_try_board, hu_own_board);
} 


void battleship::print_full_game_data(string outstr) const{
  std::cout << outstr << std::endl;
  std::cout << "    MY  SHIPS                MY  SHOTS" << std::endl;
  print_two_boards(pc_own_board, pc_try_board);
} 
