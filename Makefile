all: curses

curses:
	g++ -o minesweeper ./src/main.cpp -lncurses
