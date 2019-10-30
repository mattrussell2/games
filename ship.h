//ship.h 
//by matt russell
//2019

#include<iostream>
#include<string>
#include<vector> 

using namespace std;

struct pos{
  int x;
  int y;
  
  pos() {
    this->x = -1;
    this->y = -1;
  }
  pos(int x, int y){
    this->x = x;
    this->y = y;
  }
  pos(const pos &p){
    this->x = p.x;
    this->y = p.y;
  }
  bool operator==(pos p){
    return (p.x == this->x && p.y == this->y);
  }
};

class ship{
 
 public:
  ship();
  ship(string s, string c, pos p, int l, bool d);
  ~ship() {};
  
  string get_name() const { return name; };
  string get_char() const { return c; }; 
  int get_length() const { return length; };
  bool get_dir() const { return dir; };
  bool is_sunk() const;
  pos get_start_pos() const { return start_pos; };
  int get_x() const { return start_pos.x; };
  int get_y() const { return start_pos.y; };
  void print() const;
  void set_hits();
  bool within_ship(pos p) const;
  void add_hit_to_ship(pos p);

 private:

  bool matches_horizontal(pos p) const;
  bool matches_vertical(pos p) const;
  bool within_horizontal(pos p) const;
  bool within_vertical(pos p) const;
  
  string c;  
  string name;        //name
  int length;         //length of ship
  bool dir;           //direction of ship (1 is vertical, 0 horizontal)
  string direction;
  pos start_pos;
  bool sunk;          //true if the ship is sunk
  vector<bool> hits;  //vector of hits (0 not hit, 1 hit)
};
