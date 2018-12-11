#include "battleship.h"

int main(){
  battleship *b = new battleship;
  b->run_game();
  delete b;
  b = NULL;
}
