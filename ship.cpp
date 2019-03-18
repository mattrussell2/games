//ship.cpp
//by matt russell
//2019

#include "ship.h"

ship::ship(){
  name="";
  c=' ';
  start_x = -1; 
  start_y = -1;
  length = -1;
  dir = -1; 
}

ship::ship(string n,string ch, int x, int y, int l, bool d){
  name = n;
  c=ch;
  start_x = x; 
  start_y = y;
  length = l;
  dir = d;
 
  sunk=false;
  hits = vector<int>();
  for (int i=0;i<length;i++){
    hits.push_back(0);
  }
}

bool ship::within_ship(int x, int y) const{
  if (dir==0){
    if ((start_y==y) && (x >= start_x) && (x < start_x+length)){
      return true;
    }    
  }
  else {
    if ((start_x==x) && (y >= start_y) && (y < start_y+length)){
      return true;
    }
  }
  return false;
}

//adds a value we know to be within the ship as a hit
//sets the appropriate boolean tick in hits[]
void ship::add_hit_to_ship(int x, int y){
  if (dir==0) hits.at(x-start_x) = 1;
  else hits.at(y-start_y) = 1;
}

void ship::set_hits(){
  sunk=false;
  hits=vector<int>(length);
  for (int i=0;i<length;i++){
    hits.push_back(0);
  }
}

void ship::print() const {
  cout << "c: " << c << " length: " << length << " start x: " << start_x << " start_y: " << start_y << " dir: " << dir << " is sunk? " << is_sunk() << endl;
}

bool ship::is_sunk() const {
  for (int i=0;i<length;i++){
    if (hits.at(i)==0) return false;
  }
  return true;
}
