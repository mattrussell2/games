#include<iostream>
#include<string>
#include "ship.h"

using namespace std;


class board{

 public:

 private:
  vector<int> probabilities; //1-100. p=-1 if already hit, -2 if miss, -3 if a sunk ship
  vector<int> hits;
  
  vector<ship> ships;
  

}

