all: curses

curses:
	g++ -o minesweeper main.cpp -lncurses
