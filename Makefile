src = ./src/main.cpp
objs = $(src:.cpp=.o)
CXXFLAGS = -Wall -lncurses

.PHONY: all curses

all: curses

curses:
	$(CXX) -o minesweeper $(src) $(CXXFLAGS)

objects: $(objs)
	$(CXX) -o minesweeper $(objs) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm $(objs)
