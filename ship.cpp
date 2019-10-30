//ship.cpp
//by matt russell
//2019

#include "ship.h"

ship::ship(){
  name = "";
  c = ' ';
  start_pos = pos();
  length = -1;
  dir = -1; 
}

ship::ship(string n,string ch, pos p, int l, bool d){
  name = n;
  c = ch;
  start_pos = p;
  length = l;
  dir = d;
  if (d == 0) direction = "horizontal";
  else        direction = "vertical";
 
  sunk = false;
  hits = vector<bool>();
  for (int i = 0; i < length; i++){
    hits.push_back(false);
  }
}

bool ship::matches_horizontal(pos p) const{
  return p.x == start_pos.x;
}

bool ship::matches_vertical(pos p) const{
  return p.y == start_pos.y;
}

bool ship::within_horizontal(pos p) const {
  return (p.x >= start_pos.x) && (p.x < start_pos.x + length);
}
bool ship::within_vertical(pos p) const {
  return (p.y >= start_pos.y) && (p.y < start_pos.y + length);
}

bool ship::within_ship(pos p) const{
  return (
      (direction == "horizontal" && matches_vertical(p) && within_horizontal(p))

      ||
      
      (direction == "vertical" && matches_horizontal(p) && within_vertical(p)));
}

//adds a value we know to be within the ship as a hit
//sets the appropriate boolean tick in hits[]
void ship::add_hit_to_ship(pos p){
  if (direction == "horizontal") hits.at(p.x - start_pos.x) = 1;
  else                           hits.at(p.y - start_pos.y) = 1;
}

void ship::set_hits(){
  sunk = false;
  hits = vector<bool>(length);
  for (int i = 0;i < length;i++){
    hits.push_back(false);
  }
}

void ship::print() const {
  cout << "c: " << c << " length: " << length << " start x: " << start_pos.x << " start_y: " << start_pos.y << " dir: " << dir << " is sunk? " << is_sunk() << endl;
}

bool ship::is_sunk() const {
  for (int i = 0;i < length;i++){
    if (hits.at(i) == false) return false;
  }
  return true;
}
