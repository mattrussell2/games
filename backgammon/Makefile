CXX = clang++
CXXFLAGS = -std=c++20

backgammon: Board.cpp Game.cpp
	${CXX} ${CXXFLAGS} -o backgammon Board.cpp Game.cpp

clean:
	rm -rf *.o *~ *.dSYM backgammon