#include <cstring>
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

struct Dims{
public:
    int height;
    int width;
    int mines;
    Dims(){
    }
    Dims(int _height, int _width, int _mines){
        height = _height;
        width = _width;
        mines = _mines;
    }
};

struct Button{
public:
    int i,j;
    string text;
    Button(){
    }
    Button(int _i, int _j, string _text){
        i = _i;
        j = _j;
        text = _text;
    }
};

WINDOW* create_win(int height, int width, int starty,int startx);
void destroy_win(WINDOW* local_win);

Dims mode_select(Draw draw){
    int height = 20;
    int width = 35;
    int starty = (LINES - height) / 2;
    int startx = (COLS - width) / 2;

    curs_set(0);

    refresh();
    WINDOW* win = create_win(height, width, starty, startx);

    Button buttons[4];
    Dims dims[4];

    dims[0].height = 9;
    dims[0].width = 9;
    dims[0].mines = 10;

    dims[1].height = 16;
    dims[1].width = 16;
    dims[1].mines = 40;

    dims[2].height = 16;
    dims[2].width = 30;
    dims[2].mines = 99;
    
    dims[3].height = 9;
    dims[3].width = 9;
    dims[3].mines = 10;




    buttons[0].text = "BEGINNER";
    buttons[1].text = "INTERMEDIATE";
    buttons[2].text = "EXPERT";
    buttons[3].text = "CUSTOM";

    for(int i = 0;i < 4;i++){;
        buttons[i].i = i+2 * 2;
        buttons[i].j = 2;
    }

    wmove(win, 0, 1);
    wprintw(win, "GAME SELECTION");
    wmove(win, 2, 13);
    wprintw(win, "HEIGHT  WIDTH  MINES");
    wmove(win, buttons[0].i, buttons[0].j);
    wprintw(win, "%s       9      9     10", buttons[0].text.c_str());
    wmove(win, buttons[1].i, buttons[1].j); 
    wprintw(win, "%s  16     16     40", buttons[1].text.c_str());
    wmove(win, buttons[2].i, buttons[2].j);
    wprintw(win, "%s        16     30     99", buttons[2].text.c_str());
    wmove(win, buttons[3].i, buttons[3].j);
    wprintw(win, "%s", buttons[3].text.c_str());

    int* selected = new int[4];
    memset(selected, 0, sizeof(int) * 4);

    selected[0] = 1;
    int curent_selected = 0;
    
    for(int i = 0;i < 4;i++){
        wmove(win, buttons[i].i, buttons[i].j);
        if(selected[i] == 1){
            wattron(win, COLOR_PAIR(WHITE));
        }else{
            wattroff(win, COLOR_PAIR(WHITE));
        }
        wprintw(win, "%s", buttons[i].text.c_str());
    }

    wrefresh(win);

    Dims dim;
    
    int ch;
    while((ch = getch())){
        if(ch == 'q'){
            break;
        }
        switch(ch){
            case KEY_DOWN:
            case 's':
                selected[curent_selected] = 0;
                wattroff(win, COLOR_PAIR(WHITE));
                wmove(win, buttons[curent_selected].i, buttons[curent_selected].j);
                wprintw(win, "%s", buttons[curent_selected].text.c_str());
                curent_selected = (curent_selected >= 3) ?
                    0: curent_selected + 1;
                selected[curent_selected] = 1;
                break;
            case KEY_UP:
            case 'w':
                selected[curent_selected] = 0;
                wattroff(win, COLOR_PAIR(WHITE));
                wmove(win, buttons[curent_selected].i, buttons[curent_selected].j);
                wprintw(win, "%s", buttons[curent_selected].text.c_str());
                curent_selected = (curent_selected <= 0) ?
                    3: curent_selected - 1;
                selected[curent_selected] = 1;
                break;
            case '\n':
                dim = dims[curent_selected];
                break;
        }
        if(ch == '\n'){
            wclear(win);
            break;
        }
        wattron(win, COLOR_PAIR(WHITE));
        wmove(win, buttons[curent_selected].i, buttons[curent_selected].j);
        wprintw(win, "%s", buttons[curent_selected].text.c_str());
        wrefresh(win);
    }

    wattroff(win, COLOR_PAIR(WHITE));
    wrefresh(win);
    destroy_win(win);

    delete [] selected;
    return dim;
}

WINDOW* create_win(int height, int width, int starty,int startx){
    WINDOW* local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);

    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW* local_win){
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}

const int offset_i = 2;
const int offset_j = 0;

void draw_board(Board& board,Draw draw){
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

int main(int argc, char* argv[]){
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    noecho();

    Draw draw;

    Dims dim = mode_select(draw);
    clear();

    int height = dim.height,width = dim.width,mines = dim.mines;
    run_script("random_api.py",mines,height,width);
    Board board(height,width);
    board.init_map();
    board.set_mines_from_file("random_cords.txt");
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
                draw.setxy(0,height+offset_i+1);
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
                draw.setxy(0,height+offset_i+1);
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
    draw.setxy(0, height+offset_i+3);
    printw("Press Any key to exit");
    getch();
    endwin();
    return 0;
}
