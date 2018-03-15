#include "battleship.h"
#include <algorithm>
#include <fstream>
#include <iostream>

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
  cout << "Let\'s play! You have the option to place the ships on the board, or have " << endl;
  cout << "me do it for you (don\'t worry, I won\'t peek!)" << endl;

  pc_try_board = new board();    hu_try_board = new board();
  pc_own_board = new board();    hu_own_board = new board();

  init_board(pc_try_board);      init_board(hu_try_board);  
  init_board(pc_own_board);      init_board(hu_own_board);

  pc_ships = new vector<ship>(); 
  hu_ships = new vector<ship>();
  
  string s;
  cout << "Would you like to place the ships on the board yourself (y/n)?";
  getline(cin,s);
  while (s!="y" && s!="n") { 
    cout << "Enter y or n"; 
    getline(cin,s);
  }    
  cout << endl;
  if (s=="y") {
    human_generate_ships(hu_own_board,hu_ships);
  } else{
    generate_ships(hu_own_board,hu_ships);  
  }
  generate_ships(pc_own_board,pc_ships);  
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
      if (pc_try_board->at(i).at(j)!="*" || pc_try_board->at(i).at(j)!="_") pc_count++;
      if (hu_try_board->at(i).at(j)!="*" || hu_try_board->at(i).at(j)!="_") usr_count++;
    }
  }
  if (pc_count==44 || usr_count==44) return true;
  else return false;
}

//adds the hit to the vector of ships; if a ship is sunk, display info
void battleship::add_hit_to_ships(board *&b,vector<ship> *&ships,int x, int y){
  int q = (int)ships->size();
  for (int i=0;i<q;i++){
    if (ships->at(i).within_ship(x,y)){
      ships->at(i).add_hit_to_ship(x,y);
      if (ships->at(i).is_sunk()) {
	ship s = ships->at(i);
	cout << "ARGH! YOUVE SUNK ME SHIP: ";
	char c = s.get_char();
	if (c=='A') cout << "DRATS! An Aircraft Carrier!" << endl;
	if (c=='B') cout << "OH NO! A Battleship!" << endl;
	if (c=='C') cout << "EGADS! A Cruiser!" << endl;
	if (c=='S') cout << "GOODNESS! A Submarine!" << endl;
	if (c=='D') cout << "RATS! A Destroyer" << endl;        
	
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

void battleship::make_guess(){
  int x;
  int y;
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
    cout << "Human guesses x: " << x+1 << " and y: " << y+1 << " ";
    if (pc_own_board->at(y).at(x)!="*") {
      if (pc_own_board->at(y).at(x)=="\033[1;31mX\033[0m"){
	cout << "already hit here!" << endl;	
      }
      if (pc_own_board->at(y).at(x)=="\033[1;34m_\033[0m"){
	cout << "already missed here!" << endl;	
      }else{ 
	cout << "\033[1;31m hit!\033[0m" << endl;
	register_hit(pc_own_board,hu_try_board,x,y); 
	add_hit_to_ships(hu_try_board,pc_ships,x,y);           
      }
    }else {
      cout << "\033[1;34m miss!\033[0m" << endl;
      register_miss(pc_own_board,hu_try_board,x,y); 
    }
  }
}
//}

void battleship::easy_guess(int &x, int &y){
  x = rand()%10;
  y = rand()%10;
}


//still to be implemented.
//look for hit ships. if one is found, x,y will be updated accordingly, and return true. 
//otherwise, set x and y to best guess and return false. 
bool battleship::search_for_hit_ships(int &x, int &y){
  x=-1; y=-1;
  for (int i=0;i<10;i++){
    for (int j=0;j<10;j++){
      if (pc_try_board->at(y).at(x)=="\033[1;31mX\033[0m"){
	//return these x,y vals. find the best are to hit around them.
      }
    }
  }
  return false;
}

//still to implement
void battleship::medium_guess(int &x, int &y){
  x=5;y=5;
  //search for unsunk ships that are hit. if found, target them. 
  
  //while(pc_try_board->at(y).at(x){
  //  }
}

void battleship::pc_make_guess(){
  int x; int y;
  easy_guess(x,y); // medium_guess(x,y); //implement medium guessing and letting user pick difficulty

  //cout << "Time for me to Fire!"<< endl;
  cout << "PC is guessing x: " << x+1  << " and y: " << y+1 << " ";
  if (hu_own_board->at(y).at(x)!="*") {
    cout << "\033[1;31m hit!\033[0m" << endl;
    register_hit(hu_own_board,pc_try_board,x,y); 
    add_hit_to_ships(hu_own_board,hu_ships,x,y); //show the user that their ships are hit
  }else {
    cout << "\033[1;34m miss!\033[0m" << endl;
    register_miss(hu_own_board,pc_try_board,x,y);
  }
  cout << endl;
}

//continue allowing the user to guess until one player wins
void battleship::run_game(){ 
  while(!gameover()){
    make_guess();   
    pc_make_guess();    
    print_user_game_data();
    cout << endl;
  }
}


//checks if a given area is clear of ships
bool battleship::check_clear_area(board *board,int len, int x, int y, int d){
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
void battleship::find_valid_start_loc(board *board, int len, int &x, int &y, bool &d){  
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

void battleship::get_start_coord(board *b, int len, int &x, int &y, bool &d){
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
  cout << "   " << 12345678910 << endl;
}

void print_header();
void print_header(){
  cout << "   YOUR SHOTS             YOUR SHIPS" << endl;
}

void battleship::print_board(board *b) const{
  print_digits();
  for (int i=0;i<10;i++){
    cout << " " << char(65+i) << " ";
    for (int j=0;j<10;j++){
      cout << b->at(i).at(j);
    }
    cout << " " << char(65+i) << endl; 
  }    
  // cout << 10 << " ";
  // for (int i=0;i<10;i++){
  //  cout << b->at(9).at(i);
  // }
  //  cout << " " << "Z"10 << endl;  
}

void battleship::print_user_game_data() const{
  print_header();
  print_digits();
  for (int i=0;i<10;i++){
    cout << " " << char(65+i) << " ";
    for (int j=0;j<10;j++){
      cout << hu_try_board->at(i).at(j);
    }
    cout << " " << char(65+i) << "           ";
    
    for (int j=0;j<10;j++){
      cout <<  hu_own_board->at(i).at(j);
    }
    cout << endl;
  }    
  /*cout << 10 << " ";
  for (int i=0;i<10;i++){
    cout << hu_try_board->at(9).at(i);
  }
  cout << " " << 10 << "          ";

  for (int j=0;j<10;j++){
    cout <<  hu_own_board->at(9).at(j);
  }  
  cout << endl;*/
} 
