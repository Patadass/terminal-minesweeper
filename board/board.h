#include <fstream>

#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

struct Map{
private:
    int height,width;
public:
    short* map;
    Map();
    Map(int _height,int _width);
    void set(int _height,int _width);
    ~Map();
};

class Board{
private:
    int height,width;
    Map map;
    Map blanket;
    std::fstream file;
public:
    Board();
    Board(int _height,int _width);

    bool is_out_of_bounds(int i,int j);
    bool is_out_of_bounds_raw(int i);

    void init_map();
    void init_blanket();
    void set_bombs_from_file(std::string f_name);
    int get_from_csv(std::string line,int n);

    int get_height();
    int get_width();
    short get_map(int i,int j);
    short get_map_raw(int i);
    short get_blanket(int i,int j);
    void set_map(int i,int j,short n);
    void set_blanket(int i,int j,short n);
    void set_map_raw(int i,short n);
    void set_blanket_raw(int i,short n);

    void refresh_blanket();

    void print_map();

    void select(int i,int j);

    short operator()(int i,int j);

    void operator()(int i,int j,short n);

};

#endif //MINESWEEPER_BOARD_H
