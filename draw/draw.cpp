#include <iostream>
#include "draw.h"

HANDLE Draw::h_console = GetStdHandle(STD_OUTPUT_HANDLE);

void Draw::show_console_cursor(bool show) {
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(h_console,&cursor_info);
    cursor_info.bVisible = show;
    SetConsoleCursorInfo(h_console,&cursor_info);
}

BOOL Draw::setxy(short x, short y) {
    COORD c = {x,y};
    return SetConsoleCursorPosition(h_console, c);
}
BOOL Draw::setxy(int x, int y) {
    return setxy(short(x),short(y));
}
void Draw::set_color(WORD w) {
    SetConsoleTextAttribute(h_console,w);
}
void Draw::reset_color() {
    SetConsoleTextAttribute(h_console, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
}

void Draw::draw_fill_rect(int x, int y, int width, int height) {
    for(int i = x;i < width+x;i++){
        for(int j = y;j < height+y;j++){
            setxy(i, j);std::cout<<" "<<std::endl;
        }
    }
}

void Draw::draw_rect(int x, int y, int width, int height) {
    for(int i = x;i < width+x;i++){
        for(int j = y;j < height+y;j++){
            if(i == x || i+1 == width+x || j == y || j+1 == height+y){
                setxy(i, j);std::cout<<" "<<std::endl;
            }
        }
    }
}