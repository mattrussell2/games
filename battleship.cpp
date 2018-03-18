#include "battleship.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;



void battleship::start_game(){
  cout << R"(
             ___.              __     __   .__                   .__     .__         
	     \_ |__  _____   _/  |_ _/  |_ |  |    ____    ______|  |__  |__|______
              | __ \ \__  \  \   __\\   __\|  |  _/ __ \  /  ___/|  |  \ |  |\____ \
              | \_\ \ / __ \_ |  |   |  |  |  |__\  ___/  \___ \ |   Y  \|  ||  |_> >
              |___  /(____  / |__|   |__|  |____/ \___  >/____  >|___|  /|__||   __/  
                  \/      \/                          \/      \/      \/     |__|     
	    )" << endl;

  cout << "Welcome to battleship!" << endl;
  cout << "You have the option to place the ships on the board, or have " << endl;
  cout << "me do it for you." << endl;

  string s;
  cout << "Would you like to place the ships on the board yourself (y/n)? ";
  getline(cin,s);
  while (s!="y" && s!="n" && s!="Y" && s!="N") { 
    cout << "please enter y/n: ";
    getline(cin,s);
  }    
  cout << endl;

  srand(time(NULL));
  if (s=="y" || s=="Y") {
    generate_ships(hu_own_board,hu_ships,true);
  } else{
    generate_ships(hu_own_board,hu_ships,false);  
  }
  generate_ships(pc_own_board,pc_ships,false);  
 
  cout << "Okay! How difficult would you like me? (0 = easy, 1 = medium, 2 = hard): ";
  getline(cin,s);
  while(s!="0" && s!="1" && s!="2"){
    cout << "Enter 0 or 1 or 2: ";
    getline(cin,s);
  }
  if (s=="0") difficulty="easy";
  if (s=="1") difficulty="medium";
  if (s=="2") difficulty="hard";
  cout << endl;
  print_user_game_data();
  cout << endl;
}

battleship::battleship(){
  pc_pboard = new pboard();      hu_pboard = new pboard();

  pc_try_board = new board();    hu_try_board = new board();
  pc_own_board = new board();    hu_own_board = new board();

  init_board(pc_try_board);      init_board(hu_try_board);  
  init_board(pc_own_board);      init_board(hu_own_board);
  
  init_pboard(pc_pboard);        init_pboard(hu_pboard);

  pc_ships = new vector<ship>(); 
  hu_ships = new vector<ship>();
  
}

void battleship::init_pboard(pboard *&pb){
  vector<int> a;
  for (int i=0;i<10;i++){
    pb->push_back(a);
    for (int j=0;j<10;j++){
      pb->at(i).push_back(0);
    }
  }
}

void battleship::init_board(board *&b){
  vector<string> v;
  for (int i=0;i<10;i++){
    b->push_back(v);
    for (int j=0;j<10;j++){
      b->at(i).push_back("*");
    }
  }
} 

//test if all hits have occured.
bool battleship::gameover() const{
  int pc_count = 0;
  int usr_count = 0;
  for (int i=0;i<10;i++){    
    for (int j=0;j<10;j++){
      if (pc_try_board->at(i).at(j)!="*" && pc_try_board->at(i).at(j)!="\033[1;34m_\033[0m" && pc_try_board->at(i).at(j)!="\033[1;31mX\033[0m") pc_count++;
      if (hu_try_board->at(i).at(j)!="*" && hu_try_board->at(i).at(j)!="\033[1;34m_\033[0m" && hu_try_board->at(i).at(j) !="\033[1;31mX\033[0m") usr_count++;
    }
  }
  if (pc_count==17){ 
    cout <<"I WIN. BWAHAHAHAHAA!!!!"<<endl; 
    print_full_game_data();
    return true;
  }
  if (usr_count==17){ 
    cout <<"YOU WIN. NOOOOOOOOOOOO!!!" << endl; 
    print_full_game_data();
    return true;
  }
  return false;
}

//adds the hit to the vector of ships; if a ship is sunk, display info
void battleship::add_hit_to_ships(board *&b,vector<ship> *&ships, pboard *&pb,int x, int y, bool disp){
  int q = (int)ships->size();
  for (int i=0;i<q;i++){
    if (ships->at(i).within_ship(x,y)){
      ships->at(i).add_hit_to_ship(x,y);
      if (ships->at(i).is_sunk()) {
	ship s = ships->at(i);
	if (disp){
	  cout << "ARGH! YOUVE SUNK ME SHIP: ";
	  char c = s.get_char();
	  if (c=='A') cout << "DRATS! An Aircraft Carrier!" << endl;
	  if (c=='B') cout << "OH NO! A Battleship!" << endl;
	  if (c=='C') cout << "EGADS! A Cruiser!" << endl;
	  if (c=='S') cout << "GOODNESS! A Submarine!" << endl;
	  if (c=='D') cout << "RATS! A Destroyer" << endl;        
	}
	string sunk_string = "\033[1;31m" + s.get_name().substr(0,1) + "\033[0m";
	//replace the ship on the try board with the char of the ship
	place_ship(b,s.get_length(), s.get_x(), s.get_y(), s.get_dir(),sunk_string); 
	pb->at(s.get_x()).at(s.get_y())=-2; //sunk probability
	return;
      }
    }    
  }
}

void battleship::register_hit(board *&hit_board,board *&firing_board,pboard *& f_pboard,int x, int y){
  hit_board->at(y).at(x) = "\033[1;31mX\033[0m";
  firing_board->at(y).at(x) = "\033[1;31mX\033[0m"; 
  f_pboard->at(y).at(x) = -1;
}

void battleship::register_miss(board *&missed_board,board *&firing_board, pboard *& f_pboard, int x, int y){
  missed_board->at(y).at(x)="\033[1;34m_\033[0m";
  firing_board->at(y).at(x)="\033[1;34m_\033[0m";
  f_pboard->at(y).at(x) = -3;
}

void battleship::hu_make_guess(){
  int x; int y;
  string input;
  cout << "Enter a coordinates (i.e. A3): ";
  getline(cin,input);
  y = int(toupper(input[0]))-65;
  if (input.length()>2){
    x = ((int)(toupper(input[1]))-48)*10+(int)(toupper(input[2]))-48;
  }else x = (int)(toupper(input[1]))-48;
  x--; //convert from 'human readable' to 'machine format'
  
  if (x>9 || y > 9 || x < 0 || y < 0) { 
    cout << "please enter valid coordinates." << endl; 
  }
  else{
    cout << "Human guesses: " << input << " ";
    if (pc_own_board->at(y).at(x)!="*") {
      if (pc_own_board->at(y).at(x)=="\033[1;31mX\033[0m"){
	cout << "already hit here!" << endl;	
      }
      if (pc_own_board->at(y).at(x)=="\033[1;34m_\033[0m"){
	cout << "already missed here!" << endl;	
      }else{ 
	cout << "\033[1;31m hit!\033[0m" << endl;
	register_hit(pc_own_board,hu_try_board,hu_pboard,x,y); 
	add_hit_to_ships(hu_try_board,pc_ships,hu_pboard,x,y,1);           
      }
    }else {
      cout << "\033[1;34m miss!\033[0m" << endl;
      register_miss(pc_own_board,hu_try_board,hu_pboard,x,y); 
    }
  }
}

void battleship::easy_guess(board *b, int &x, int &y) const{
  srand(time(NULL));
  x = rand()%10;
  y = rand()%10;
  while (b->at(y).at(x)!="*"){
    x=rand()%10;
    y=rand()%10;
  }
}

//look for hit ships. if one is found, x,y will be updated accordingly, and return true. 
bool battleship::search_for_hit_location(board *b, int &x, int &y) const{
  int converted = y*10+x;
  for (int i=0;i<10;i++){
    for (int j=0;j<10;j++){
      if (i*10+j < converted) continue;
      if (b->at(i).at(j)=="\033[1;31mX\033[0m"){
	y=i; x=j;
	return true;
      }
    }
  }
  return false;
}

//checks to see if a given location is valid to fire;
bool battleship::is_valid_loc(board *b,int x, int y) const{
  if (x>9 || y > 9 || x < 0 || y < 0) return false;
  if (b->at(y).at(x)=="*") return true;
  else return false;      
}


//pre: a ship has already been hit. 
//post: x and y are set to a new location to aim for.
bool battleship::blood_in_the_water(board *b,int &x, int &y) const{
  if (is_valid_loc(b,x+1,y)) {x++; return true;}
  if (is_valid_loc(b,x-1,y)) {x--; return true;}
  if (is_valid_loc(b,x,y+1)) {y++; return true;}
  if (is_valid_loc(b,x,y-1)) {y--; return true;}
  return false;//this location may not be viable (another spot on ship is...)
}
      
//finds an okay spot, given that a ship has been hit already
void battleship::find_medium_good_spot(board *b, int &x, int &y) const{
  // loop until finding an x to aim around
  bool valid=false;
  while (!valid){
    search_for_hit_location(b,x,y); //now x and y are valid coordinate of hit. #no return
    valid = blood_in_the_water(pc_try_board,x,y); //if we can work it, we're done.
    if (!valid){ //have to increment the position for next call to search
      x++;
      if (x>9) {x=0;y++;}
      if (y>9) {cout << "uh oh. can't find a valid spot to shoot. (contact developer and shame him for his idiocy, please)." << endl;	
      }
    }
  }  
}

//checks if a given area is clear of ships
bool battleship::check_nearby_hit(board *board,int len, int x, int y, int d) const{
 if (d==0){
    for (int z=x;z<x+len;z++){
      if (board->at(y).at(z)=="\033[1;31mX\033[0m") return true;
    }
  }
  else if (d==1){
    for (int z=y;z<y+len;z++){
      if (board->at(z).at(x)=="\033[1;31mX\033[0m") return true;
    }
  }
  return false;
}

void battleship::could_hit_ship(board *b,pboard *&pb, int x, int y, int len, bool d) {
  int modifier;
  if (d==0){ //horizontal
    if (x+len-1<=9 && check_clear_area(b,len,x,y,0)){ 
      if (check_nearby_hit(b,len,x,y,0)) { modifier=50; } else modifier=1;
      for (int i=0;i<len;i++){
	pb->at(y).at(x+i)+=modifier;
      }
    }
  }
  if (d==1){
    if (y+len-1<=9 && check_clear_area(b,len,x,y,1)) {
      if (check_nearby_hit(b,len,x,y,1)) {modifier=50;} else { modifier=1;}
      for (int i=0;i<len;i++){
	pb->at(y+i).at(x)+=modifier;
      }
    }
  }
}
    
void battleship::calc_probabilities(board *b, pboard *&pb){  
  delete pb;
  pb = new pboard();
  init_pboard(pb);  
  int len;
  for (int i=0;i<10;i++){
    for (int j=0;j<10;j++){ //for each space
      for (int z=0;z<5;z++){ //for each ship
	len = ship_len_lookup[z];
	could_hit_ship(b,pb,j,i,len,0);
	could_hit_ship(b,pb,j,i,len,1);
      }
    }
  }
}

void battleship::find_best_guess(board *b, pboard *&pb, int &x, int &y){
  calc_probabilities(b,pb);
  int largest_p=0;
  for (int i=0;i<10;i++){
    for (int j=0;j<10;j++){
      if (pb->at(i).at(j)>largest_p && is_valid_loc(b,j,i)){
	largest_p=pb->at(i).at(j);
	x=j; y=i;
      }
    }
  }
}

void battleship::pc_make_guess(){
  int x; int y; bool found;
  if (difficulty=="easy") easy_guess(pc_try_board,x,y);
  if (difficulty=="medium"){
    x=0;y=0;
    found = search_for_hit_location(pc_try_board,x,y);
    if (!found){
      easy_guess(pc_try_board,x,y);  
    }else{
      find_medium_good_spot(pc_try_board,x,y);
    }
  }
  if (difficulty=="hard") find_best_guess(pc_try_board,pc_pboard,x,y);
  
  cout << "PC guesses: " + string(1,(char)(y+65)) << x+1;
  if (hu_own_board->at(y).at(x)!="*") {
    cout << "\033[1;31m hit!\033[0m" << endl;
    register_hit(hu_own_board,pc_try_board,pc_pboard,x,y); 
    add_hit_to_ships(hu_own_board,hu_ships,pc_pboard,x,y,1); //show hits to user
    add_hit_to_ships(pc_try_board,hu_ships,pc_pboard,x,y,0);
  }else {
    cout << "\033[1;34m miss!\033[0m" << endl;
    register_miss(hu_own_board,pc_try_board,pc_pboard,x,y);
  }
  cout << endl;
}


void battleship::pc_test(){
  string diffs[3]={"easy","medium","hard"};
  int e[10000];
  int m[10000];
  int h[10000];
  int turn_counter;
  for (int i=0;i<3;i++){
    cout << "Starting difficulty: " << diffs[i] << endl;
    for (int j=0;j<10000;j++){
      difficulty=diffs[i]; //set difficulty
      pc_pboard = new pboard();      hu_pboard = new pboard();
      
      pc_try_board = new board();    hu_try_board = new board();
      pc_own_board = new board();    hu_own_board = new board();
      
      init_board(pc_try_board);      init_board(hu_try_board);  
      init_board(pc_own_board);      init_board(hu_own_board);
      
      init_pboard(pc_pboard);        init_pboard(hu_pboard);
      
      pc_ships = new vector<ship>(); 
      hu_ships = new vector<ship>();
      
      generate_ships(hu_own_board,hu_ships,0);  
      generate_ships(pc_own_board,pc_ships,0);  
    
      turn_counter=0;
      while(!gameover()){
	pc_make_guess();
	turn_counter++;
      }
      if (i==0) e[j]=turn_counter;
      if (i==1) m[j]=turn_counter;
      if (i==2) h[j]=turn_counter;
    }
  }
  int sum;
  double avg;
  for (int i=0;i<3;i++){
    sum=0; 
    for (int j=0;j<10000;j++){
      if (i==0) sum+=e[j];
      if (i==1) sum+=m[j];
      if (i==2) sum+=h[j];
    }
    avg = sum / 10000.0;
    cout << "For difficulty difficult the average number of turns was: " << avg << endl;
  }
}
  

//continue allowing the user to guess until one player wins
void battleship::run_game(){ 
  int turn_counter=0;
  start_game();
  while(!gameover()){
    hu_make_guess();   
    pc_make_guess();    
    print_user_game_data();
    cout << endl;
    turn_counter++;
  }
  cout << "The game was finished in " << turn_counter << " turns." << endl;
}


//checks if a given area is clear of ships
bool battleship::check_clear_area(board *board,int len, int x, int y, int d) const{
  int count=0;
  if (d==0){
    for (int z=x;z<x+len;z++){
      if (board->at(y).at(z)=="*" || board->at(y).at(z)=="\033[1;31mX\033[0m") count++;
    }
  }
  else if (d==1){
    for (int z=y;z<y+len;z++){
      if (board->at(z).at(x)=="*" || board->at(z).at(x)=="\033[1;31mX\033[0m") count++;
    }
  }
  if (count==len) return true;
  else return false;
}

//i is ship length; x is x loc, y is y loc, d is direction (1 vertical, 0 horizontal)
//returns when it finds a valid placement position for the given ship
void battleship::find_valid_start_loc(board *board, int len, int &x, int &y, bool &d) const{  
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
void battleship::place_ship(board *&b,int len, int x, int y, bool d,string s){
  if (d==0) {
    for (int z=x;z<x+len;z++){
      b->at(y).at(z) = s;
    }
  }
  if (d==1) {
    for (int z=y;z<y+len;z++){
      b->at(z).at(x) = s;
    }    
  }
}


bool battleship::game_ready(board *human_board) const {
  int usr_star_count = 0;
  for (int i=0;i<10;i++){
    for (int j=0;j<10;j++){
      if (human_board->at(i).at(j)=="*") usr_star_count++;
    }
  }
  return (usr_star_count==66);
}

bool toBool(string s);
bool toBool(string s){
  return (s!="0");
}

//gets starting coordinate from human
void battleship::get_start_coord(board *b, int len, int &x, int &y, bool &d) const{
  bool badinput=true;
  string coord;
  while(badinput){
    cout << "Enter a starting coordinate (i.e. J5): ";
    
    getline(cin,coord);
    //    cout << "input: " << input << endl;
    y = int(coord[0])-65;
    if (coord.length()>2){
      x = ((int)coord[1]-48)*10+(int)coord[2]-48;
    }else x = (int)coord[1]-48;
    x--; //convert human input to board 
    if (x < 0 || x > 9 || y < 0 || y > 9) {
      cout << "Bad coordinate. Please try again." << endl;
      continue;
    }
    else badinput=false;
  }
  cout << "Enter a direction (0 for horizontal, 1 for vertical): "; 
  getline(cin,coord);
  d = toBool(coord);
  cout << "D: " << d << endl;
  if ((d==0 && (x+len-1>9)) ||  (d==1 && (y+len-1>9))){
    cout << "Ship will fall off the board. Please try again." << endl;
    get_start_coord(b, len, x, y, d);
  }
    
  if (!check_clear_area(b, len, x, y, d)){
    cout << "Already a ship there! Please try again." << endl;
    get_start_coord(b, len, x, y, d);
  }
}

void battleship::generate_ships(board *&b, vector<ship> *&ships, bool hu_pc){
  int x; int y; int len; bool d;
  string ship_name; char ship_class; ship s;
  for (int i=0;i<5;i++){
    len = ship_len_lookup[i];
    ship_class = ship_class_lookup[i];
    ship_name = ship_name_lookup[i];
    if (hu_pc==1){
      cout << "Placing ships of type " << ship_name << " (length: " << len << ")" << endl;
      print_board(b);
      get_start_coord(b,len,x,y,d);      
    }else find_valid_start_loc(b,len,x,y,d);
    place_ship(b,len,x,y,d,string(1,ship_name.at(0)));
    s = ship(ship_name,ship_class,x,y,len,d);
    s.set_hits();
    ships->push_back(s);
  }
}

void print_digits();
void print_digits(){
  cout << "   \033[1;32m12345678910\033[0m              \033[1;32m12345678910\033[0m\n";
}

void print_header();
void print_header(){
  cout << "   YOUR SHOTS               YOUR SHIPS" << endl;
}

string green_char(int i);
string green_char(int i){
  return "\033[1;32m" + string(1,char(65+i)) + "\033[0m";
}

void battleship::print_board(board *b) const{
  cout << "   Your board" << endl;
  cout << "   \033[1;32m12345678910\033[0m" << endl;
  for (int i=0;i<10;i++){
    cout << " " << char(65+i) << " ";
    for (int j=0;j<10;j++){
      cout << b->at(i).at(j);
    }
    cout << " " << green_char(i) << endl; 
  }    
}

void battleship::print_two_boards(board *b1, board*b2) const{
  print_digits();
  for (int i=0;i<10;i++){
    cout << " " << green_char(i) << " ";
    for (int j=0;j<10;j++){
      cout << b1->at(i).at(j);
    }
    cout << " " << green_char(i) << "           " << green_char(i) << " ";
    
    for (int j=0;j<10;j++){
      cout <<  b2->at(i).at(j);
    }
    cout << " " << green_char(i) << endl;
  }
}

void battleship::print_user_game_data() const{
  print_header();
    print_two_boards(hu_try_board,hu_own_board);
} 


void battleship::print_full_game_data() const{
  cout << "    MY  SHOTS                MY  SHIPS" << endl;
  print_two_boards(pc_try_board,pc_own_board);
} 
