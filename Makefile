.PHONY: checkfile

all: Main
	rm Game.o
	clear

#declare variables
FILE:= game.o
FILE_EXISTS:= false

#sets FILE_EXISTS to true or false depending on whether the file exists or not
checkfile:
	$(eval FILE_EXISTS := $(shell if [ -e $(FILE) ]; then echo "true"; else echo "false"; fi))

Main: main.cpp | checkfile
	if [ $(FILE_EXISTS) = "true" ]; then \
		echo "File exists!"; \
		g++ main.cpp -o TrashMan; \
	else \
		echo "File does not exist!"; \
		$(MAKE) -C Game; \
		g++ main.cpp Game.o -o TrashMan; \
	fi
	chmod +x Harness.sh

Game:
	cd Game && $(MAKE)

Test:
	g++ test.cpp -o test
	./test