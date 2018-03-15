#include "battleship.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;
battleship::battleship(){
  cout << R"(
             ___.              __     __   .__                   .__     .__         
	     \_ |__  _____   _/  |_ _/  |_ |  |    ____    ______|  |__  |__|______
              | __ \ \__  \  \   __\\   __\|  |  _/ __ \  /  ___/|  |  \ |  |\____ \
              | \_\ \ / __ \_ |  |   |  |  |  |__\  ___/  \___ \ |   Y  \|  ||  |_> >
              |___  /(____  / |__|   |__|  |____/ \___  >/____  >|___|  /|__||   __/  
                  \/      \/                          \/      \/      \/     |__|     
	    )" << endl;

  cout << "Welcome to battleship!" << endl;
  cout << "You will be able to see your attempts board, and your board of ships." << endl;
  cout << "You and I will take turns firing at each other's board. There will " << endl;
  cout << "be a notification of whether each shot hits or misses, and the boards " << endl;
  cout << "will be updated after each round." << endl;
  cout << "Pro tip! press the CAPS-LOCK key to make entering board spaces easier ;)\n";
  cout << "Let's play! You have the option to place the ships on the board, or have " << endl;
  cout << "me do it for you (don't worry, I won't peek!)" << endl;

  pc_try_board = new board();    hu_try_board = new board();
  pc_own_board = new board();    hu_own_board = new board();

  init_board(pc_try_board);      init_board(hu_try_board);  
  init_board(pc_own_board);      init_board(hu_own_board);

  pc_ships = new vector<ship>(); 
  hu_ships = new vector<ship>();
  
  string s;
  cout << "Would you like to place the ships on the board yourself (Y/N)? ";
  getline(cin,s);
  while (s!="y" && s!="n" && s!="Y" && s!="N") { 
    cout << "Enter Y/N: "; 
    getline(cin,s);
  }    
  cout << endl;

  if (s=="y" || s=="Y") {
    human_generate_ships(hu_own_board,hu_ships);
  } else{
    generate_ships(hu_own_board,hu_ships);  
  }
  generate_ships(pc_own_board,pc_ships);  

  cout << "Okay! Also, how challenging would you like me to be? (0 for easy, 1 for medium): ";
  getline(cin,s);
  while(s!="0" && s!="1"){
    cout << "Enter 1 or 0: ";
    getline(cin,s);
  }if (s=="0") difficulty="easy";
  else difficulty="medium";
  cout << endl;
  print_user_game_data();
  cout << endl;
}

void battleship::init_board(board *&board){
  vector<string> v;
  for (int i=0;i<10;i++){
    board->push_back(v);
    for (int j=0;j<10;j++){
      board->at(i).push_back("*");
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
  if (pc_count==44){ 
    cout <<"I WIN. BWAHAHAHAHAA!!!!"<<endl; 
    print_full_game_data();
    return true;
  }
  if (usr_count==44){ 
    cout <<"YOU WIN. NOOOOOOOOOOOO!!!" << endl; 
    print_full_game_data();
    return true;
  }
  return false;
}

//adds the hit to the vector of ships; if a ship is sunk, display info
void battleship::add_hit_to_ships(board *&b,vector<ship> *&ships,int x, int y, bool disp){
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
	return;
      }
    }    
  }
}

void battleship::register_hit(board *&hit_board,board *&firing_board,int x, int y){//,bool n){
  hit_board->at(y).at(x) = "\033[1;31mX\033[0m";
  firing_board->at(y).at(x) = "\033[1;31mX\033[0m"; 
}

void battleship::register_miss(board *&missed_board,board *&firing_board,int x, int y){//, bool n){
  missed_board->at(y).at(x)="\033[1;34m_\033[0m";
  firing_board->at(y).at(x)="\033[1;34m_\033[0m";
}

void battleship::hu_make_guess(){
  int x; int y;
  string input;
  cout << "Enter a coordinates (i.e. A3): ";
  getline(cin,input);
  y = int(input[0])-65;
  if (input.length()>2){
    x = ((int)input[1]-48)*10+(int)input[2]-48;
  }else x = (int)input[1]-48;
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
	register_hit(pc_own_board,hu_try_board,x,y); 
	add_hit_to_ships(hu_try_board,pc_ships,x,y,1);           
      }
    }else {
      cout << "\033[1;34m miss!\033[0m" << endl;
      register_miss(pc_own_board,hu_try_board,x,y); 
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
//otherwise, set x and y to best guess and return false. 
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
      
//still to implement
void battleship::medium_guess(board *b, int &x, int &y) const{
  x=0;y=0; bool found; 
  found = search_for_hit_location(b,x,y);
  if (!found){ //random guess if no hits on the board.
    easy_guess(b,x,y);
    return;
  }
  //otherwise, loop until finding a good spot to aim for.  
  bool valid=false;
  while (!valid){
    found = search_for_hit_location(b,x,y); //now x and y are valid coordinate of hit.    
    valid = blood_in_the_water(pc_try_board,x,y); //if we can work it, we're done.
    if (!valid){ //have to increment the position for next call to search
      x++;
      if (x>9) {x=0;y++;}
      if (y>9) {cout << "uh oh. can't find a valid spot to shoot. (contact developer and shame him for his idiocy, please)." << endl;	
      }
    }
  }  
}

void battleship::pc_make_guess(){
  int x; int y;
  if (difficulty=="easy") easy_guess(pc_try_board,x,y);
  if (difficulty=="medium") medium_guess(pc_try_board,x,y); 

  cout << "PC guesses: " + string(1,(char)(y+65)) << x+1;
  if (hu_own_board->at(y).at(x)!="*") {
    cout << "\033[1;31m hit!\033[0m" << endl;
    register_hit(hu_own_board,pc_try_board,x,y); 
    add_hit_to_ships(hu_own_board,hu_ships,x,y,1); //show hits to user
    add_hit_to_ships(pc_try_board,hu_ships,x,y,0);
  }else {
    cout << "\033[1;34m miss!\033[0m" << endl;
    register_miss(hu_own_board,pc_try_board,x,y);
  }
  cout << endl;
}

//continue allowing the user to guess until one player wins
void battleship::run_game(){ 
  int turn_counter=0;
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
      if (board->at(y).at(z)=="*") count++;
    }
  }
  else if (d==1){
    for (int z=y;z<y+len;z++){
      if (board->at(z).at(x)=="*") count++;
    }
  }
  if (count==len) return true;
  else return false;
}

//i is ship length; x is x loc, y is y loc, d is direction (1 vertical, 0 horizontal)
//returns when it finds a valid placement position for the given ship
void battleship::find_valid_start_loc(board *board, int len, int &x, int &y, bool &d) const{  
  while (true){
    srand(time(NULL));
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

//human generates board locations for the given ships
void battleship::human_generate_ships(board *&b,vector<ship> *&ships){
  //1 carrier; 2 battleships; 3 cruisers; 4 submarines; 5 destroyers
  //while (!game_ready(b)){
  int x; int y;  int len; bool d;
  string ship_name; char ship_class; ship s;
  for (int i=0;i<5;i++){
    len = ship_len_lookup[i];
    ship_class = ship_class_lookup[i];
    ship_name = ship_name_lookup[i];
    cout << "Placing ships of type " << ship_name << " (length: " << len << ")" << endl;
    int num_ships = i+1;
    cout << "We will place " << num_ships << " of this type." << endl;
    for (int j=0;j<num_ships;j++){     
      cout<< "Placing #" << j+1 << " of type " << ship_name << " (length: " << len << ")\n";
      print_board(b);
      get_start_coord(b,len,x,y,d);      
      place_ship(b,len,x,y,d,string(1,ship_name.at(0)));
      s = ship(ship_name,ship_class,x,y,len,d);
      s.set_hits();
      ships->push_back(s);
    } 
  }
}


//generate board locations for the given ships
void battleship::generate_ships(board *&b,vector<ship> *&ships){
  //1 carrier; 2 battleships; 3 cruisers; 4 submarines; 5 destroyers
  int x;  int y; int len;  bool d;
  char ship_class;  string ship_name; ship s;
  for (int i=0;i<5;i++){
    len = ship_len_lookup[i];
    ship_class = ship_class_lookup[i];
    ship_name = ship_name_lookup[i];
    int num_ships = i+1;
    for (int j=0;j<num_ships;j++){     
      find_valid_start_loc(b,len,x,y,d);      
      place_ship(b,len,x,y,d,string(1,ship_name.at(0)));
      s = ship(ship_name,ship_class,x,y,len,d);
      s.set_hits();
      ships->push_back(s);
    }
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
  print_digits();
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
