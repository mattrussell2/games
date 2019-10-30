CXX = g++
FLAGS = -g -std=c++11

battleship: main.cpp battleship.cpp ship.cpp
	${CXX} ${FLAGS} -o battleship main.cpp battleship.cpp ship.cpp

clean:
	rm -rf *.o *~ *# battleship.dSYM battleship

