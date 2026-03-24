src = ./src/main.cpp
FLAGS = -Wall -lncurses

all: curses

curses:
	$(CXX) -o minesweeper $(src) $(FLAGS)
