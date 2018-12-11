CXX = g++
FLAGS = -Wall -Wextra -g -std=c++11

battleship: main.cpp battleship.cpp ship.cpp
	${CXX} ${FLAGS} -o battleship run.cpp battleship.cpp ship.cpp

clean:
	rm -f *.o *~ *#

