all: 
	make battleship
	make backgammon

battleship: 
	cd battleship && make && mv battleship ../build/

backgammon:
	cd backgammon && make && mv backgammon ../build/

clean:
	cd backgammon && make clean && cd ../battleship && make clean && cd .. && rm build/*
	
.PHONY: battleship backgammon