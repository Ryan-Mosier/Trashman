all: Game.o
	rm pathfinding.o
	rm Map.o

Game.o: pathfinding.o
	g++ Game.cpp -c -o Map.o
	ld -r -o Game.o Map.o pathfinding.o
	mv Game.o ..

pathfinding.o:
	g++ Pathfinding.cpp -c -o pathfinding.o