CXX = clang++
CXXFLAGS = -std=c++20

battleship: main.cpp battleship.cpp ship.cpp
	${CXX} ${CXXFLAGS} -o battleship main.cpp battleship.cpp ship.cpp

clean:
	rm -f *.o *~ battleship