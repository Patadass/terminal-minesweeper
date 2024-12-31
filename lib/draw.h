#include <ncurses.h>
#include <cstdio>

#ifndef TETRIS_DRAW_H
#define TETRIS_DRAW_H

#define BLACK 1
#define RED 2
#define GREEN 3
#define YELLOW 4
#define BLUE 5
#define MAGENTA 6
#define CYAN 7
#define WHITE 8

class Draw{
public:
    //static void show_console_cursor(bool show);

    Draw(){
        _color_pair = 0;
        init_pair(1, COLOR_BLACK, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_RED);
        init_pair(3, COLOR_BLACK, COLOR_GREEN);
        init_pair(4, COLOR_BLACK, COLOR_YELLOW);
        init_pair(5, COLOR_BLACK, COLOR_BLUE);
        init_pair(6, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(7, COLOR_BLACK, COLOR_CYAN);
        init_pair(8, COLOR_BLACK, COLOR_WHITE);
    }

    void clear_screen(){
        clear();
    }

    void setxy(short x, short y){
        move(y, x);
    }

    void setxy(int x,int y){
        move(y, x);
    }

    void reset_color(){
        if(_color_pair == 0){
            return;
        }
        attroff(COLOR_PAIR(_color_pair));
    }

    void set_color(int w){
        _color_pair = w;
        attron(COLOR_PAIR(w));
    }
    

    void draw_fill_rect(int x, int y, int width,int height){
        for(int i = x;i < width+x;i++){
            for(int j = y;j < height+y;j++){
                setxy(i, j);
                printw(" \n");
            }
        }
        refresh();
    }

    void draw_rect(int x,int y,int width,int height){
        for(int i = x;i < width+x;i++){
            for(int j = y;j < height+y;j++){
                if(i == x || i+1 == width+x || j == y || j+1 == height+y){
                    setxy(i, j);
                    printw(" \n");
                }
            }
        }
        refresh();
    }
private:
    int _color_pair;
};

#endif //TETRIS_DRAW_H
