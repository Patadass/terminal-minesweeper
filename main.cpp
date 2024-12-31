#include <ncurses.h>
#include <string>
#include "lib/draw.h"
#include "lib/board.h"

using namespace std;


void run_script(const string& src,int n,int height,int width){
    const string _n = to_string(n), _height = to_string(height), _width = to_string(width);
    const string argv = _n + " " + _height + " " + _width;
    const string _cmd = "python3 " + src + " " + argv;
    char cmd[_cmd.size()];
    cmd[_cmd.size()] = ' ';
    for(int i = 0;i < _cmd.size();i++){cmd[i] = _cmd[i];}
    system(cmd);
}

void draw_board(Board& board,Draw draw){
    int offset_i = 0;
    int offset_j = 0;
    for(int i = 0;i < board.get_height();i++){
        for(int j = 0;j < board.get_width();j++){
            draw.set_color(WHITE);
            draw.setxy(j+offset_j,i+offset_i);
            if(board.get_blanket(i,j) == 0){
                printw(" ");
            }
            if(board.get_blanket(i,j) == 1){//is hovered
                draw.set_color(YELLOW);
                printw(" ");
            }
            if(board.get_blanket(i,j) == 2){//is selected num
                printw("%d",board.get_map(i,j));
            }
            if(board.get_blanket(i,j) == 3){//is selected num and is hovered
                draw.set_color(YELLOW);
                printw("%d",board.get_map(i,j));
            }
            if(board.get_blanket(i,j) == 4){//is zero
                printw(".");
            }
            if(board.get_blanket(i,j) == 5){//is zero and hovered
                draw.set_color(YELLOW);
                printw(".");
            }
            if(board.get_blanket(i,j) == 6){//is flagged
                printw("F");
            }
            if(board.get_blanket(i,j) == 7){//is flagged and is hovered
                draw.set_color(YELLOW);
                printw("F");
            }
        }
    }
    refresh();
}

void draw_board_skeleton(Board& board,Draw draw){
    for(int i = 0;i < board.get_height();i++){
        for(int j = 0;j < board.get_width();j++){
            draw.setxy(j+20,i);
            printw("%d", board.get_blanket(i,j));
        }
    }
    refresh();
}

bool game_win_con(Board& board){
    for(int i = 0;i < board.get_height();i++){
        for(int j = 0;j < board.get_width();j++){
            if(board.get_map(i,j) >=1 && board.get_map(i,j) <= 8 && board.get_blanket(i,j) != 2 && board.get_blanket(i,j) != 3){
                return false;
            }
        }
    }
    return true;
}

void key_handle(int *keyCode){
    int ch = getch();
    switch(ch){
        case KEY_F(1):
        case 'q':
            *keyCode = 99;
            break;
        case KEY_UP:
        case 'w':
            *keyCode = 1;
            break;
        case KEY_LEFT:
        case 'a':
            *keyCode = 2;
            break;
        case KEY_DOWN:
        case 's':
            *keyCode =  3;
            break;
        case KEY_RIGHT:
        case 'd':
            *keyCode = 4;
            break;
        case '\n':
            *keyCode = 5;
            break;
        case ' ':
            *keyCode = 6;
            break;
        default:
            break;
    }
}

int main(){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    noecho();

    Draw draw;
    int height = 9,width = 10,bombs = 8;
    run_script("random_api.py",bombs,height,width);
    Board board(height,width);
    board.init_map();
    board.set_bombs_from_file("random_cords.txt");
    int i = 0,j = 0;
    board.set_blanket(0,0,1);
    draw_board(board, draw);
    bool game_is_over = false;
    int *keyCode = new int;
    while(true){
        *keyCode = 0;
        key_handle(keyCode);
        if(*keyCode == 99){
            draw.setxy(0, height+1);
            printw("QUIT");
            break;
        }
        if(*keyCode == 1){// W
            if(!board.is_out_of_bounds(i-1,j)){
                if(board.get_blanket(i,j) == 1){board.set_blanket(i,j,0);}
                if(board.get_blanket(i,j) == 3){board.set_blanket(i,j, 2);}
                if(board.get_blanket(i,j) == 5){board.set_blanket(i,j, 4);}
                if(board.get_blanket(i,j) == 7){board.set_blanket(i,j,6);}
                i--;
                if(board.get_blanket(i,j) == 0){
                    board.set_blanket(i,j,1);
                }
                if(board.get_blanket(i,j) == 2){board.set_blanket(i,j,3);}
                if(board.get_blanket(i,j) == 4){board.set_blanket(i,j,5);}
                if(board.get_blanket(i,j) == 6){board.set_blanket(i,j,7);}
            }
        }
        if(*keyCode == 2){//A
            if(!board.is_out_of_bounds(i,j-1)){
                if(board.get_blanket(i,j) == 1){board.set_blanket(i,j,0);}
                if(board.get_blanket(i,j) == 3){board.set_blanket(i,j, 2);}
                if(board.get_blanket(i,j) == 5){board.set_blanket(i,j, 4);}
                if(board.get_blanket(i,j) == 7){board.set_blanket(i,j,6);}
                j--;
                if(board.get_blanket(i,j) == 0){
                    board.set_blanket(i,j,1);
                }
                if(board.get_blanket(i,j) == 2){board.set_blanket(i,j,3);}
                if(board.get_blanket(i,j) == 4){board.set_blanket(i,j,5);}
                if(board.get_blanket(i,j) == 6){board.set_blanket(i,j,7);}
            }
        }
        if(*keyCode == 3){//S
            if(!board.is_out_of_bounds(i+1,j)){
                if(board.get_blanket(i,j) == 1){board.set_blanket(i,j,0);}
                if(board.get_blanket(i,j) == 3){board.set_blanket(i,j, 2);}
                if(board.get_blanket(i,j) == 5){board.set_blanket(i,j, 4);}
                if(board.get_blanket(i,j) == 7){board.set_blanket(i,j,6);}
                i++;
                if(board.get_blanket(i,j) == 0){
                    board.set_blanket(i,j,1);
                }
                if(board.get_blanket(i,j) == 2){board.set_blanket(i,j,3);}
                if(board.get_blanket(i,j) == 4){board.set_blanket(i,j,5);}
                if(board.get_blanket(i,j) == 6){board.set_blanket(i,j,7);}
            }
        }
        if(*keyCode == 4){//D
            if(!board.is_out_of_bounds(i,j+1)){
                if(board.get_blanket(i,j) == 1){board.set_blanket(i,j,0);}
                if(board.get_blanket(i,j) == 3){board.set_blanket(i,j, 2);}
                if(board.get_blanket(i,j) == 5){board.set_blanket(i,j, 4);}
                if(board.get_blanket(i,j) == 7){board.set_blanket(i,j,6);}
                j++;
                if(board.get_blanket(i,j) == 0){
                    board.set_blanket(i,j,1);
                }
                if(board.get_blanket(i,j) == 2){board.set_blanket(i,j,3);}
                if(board.get_blanket(i,j) == 4){board.set_blanket(i,j,5);}
                if(board.get_blanket(i,j) == 6){board.set_blanket(i,j,7);}
            }
        }
        if(*keyCode == 5 && !game_is_over){//RETURN
            board.set_blanket(i,j,3);
            if(board.get_map(i,j) == 9){
                draw_board(board, draw);
                draw.setxy(0,height+1);
                printw("GAME OVER");
                break;
                game_is_over = true;
            }
            if(board.get_map(i,j) == 0){
                board.select(i,j);
                board.refresh_blanket();
                board.set_blanket(i,j,5);
            }
            if(board.get_map(i,j) == -1){
                board.set_blanket(i,j, 5);
            }
            if(game_win_con(board)){
                draw_board(board, draw);
                draw.setxy(0,height+1);
                printw("WIN");
                break;
                game_is_over = true;
            }
        }
        if(*keyCode == 6){
            if(board.get_blanket(i,j) == 7){
                board.set_blanket(i,j,1);
            }else if(board.get_blanket(i,j) == 1){
                board.set_blanket(i,j,7);
            }
        }

        //draw_board_skeleton(board);
        draw_board(board, draw);
    }
    delete keyCode;
    draw.setxy(0, height+3);
    printw("Press Any key to exit");
    getch();
    endwin();
    return 0;
}
