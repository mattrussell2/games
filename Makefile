#
# Makefile for Boggle, hw6
#
CXX = g++
FLAGS = -Wall -Wextra -g -std=c++11

battleship: run.cpp battleship.cpp ship.cpp
	${CXX} ${FLAGS} -o battleship run.cpp battleship.cpp ship.cpp

clean:
	rm -f *.o *~ *#

