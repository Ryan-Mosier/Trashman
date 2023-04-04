all: Game.o
	rm pathfinding.o
	rm Map.o
	rm structures.o

Game.o: pathfinding.o
	g++ Game.cpp -c -o Map.o
	ld -r -o Game.o Map.o pathfinding.o structures.o
	mv Game.o ..

pathfinding.o: structures.o
	g++ aStar.cpp -c -o pathfinding.o

structures.o:
	g++ Structures.cpp -c -o structures.o