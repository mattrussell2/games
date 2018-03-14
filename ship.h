#include<iostream>
#include<string>
#include<vector> 

using namespace std;

struct pos{
  int x;
  int y;
  bool operator==(pos p){
    return (p.x==x && p.y==y);
  }
  pos(int a, int b){
    x=a; y=b;
  }
};

class ship{
 
 public:
  ship();
  ship(string s,char c, int x, int y,int l, bool d);
  ~ship();
  
  string get_name() const {return name;};
  char get_char() const {return c;};
  int get_length() const {return length;};
  bool get_dir() const {return dir;};
  bool is_sunk() const;    //returns whether or not it's sunk
  int get_x() const {return start_x;};
  int get_y() const {return start_y;};
  void print() const;
  void set_hits();
  bool within_ship(int x, int y) const;
  void add_hit_to_ship(int x, int y);
 private:
  
  char c;  
  string name; //name
  int length;  //length of ship
  bool dir;    //direction of ship (1 is vertical, 0 horizontal)
  int start_x; //starting horizontal direction
  int start_y; //starting pos in vertical direction
  bool sunk;   //true if the ship is sunk
  vector<int> hits;  //vector of hits (0 not hit, 1 hit)
 
};
