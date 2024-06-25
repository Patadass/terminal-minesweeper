#include <iostream>
#include <thread>
#include <string>
#include "draw/draw.h"
#include "board/board.h"

using namespace std;

void run_script(const string& src,int n,int height,int width){
    ofstream file;
    file.open("python_input.txt", ios::trunc | ios::out);
    file<<n<<"\n"<<height<<"\n"<<width;
    file.close();
    const string _cmd = "python3 " + src;
    char cmd[_cmd.size()];
    for(int i = 0;i < _cmd.size();i++){cmd[i] = _cmd[i];}
    system(cmd);
}

void draw_board(Board& board){
    int offset_i = 0;
    int offset_j = 0;
    for(int i = 0;i < board.get_height();i++){
        for(int j = 0;j < board.get_width();j++){
            Draw::set_color(BACKGROUND_WHITE);
            Draw::setxy(j+offset_j,i+offset_i);
            if(board.get_blanket(i,j) == 0){
                cout<<" ";
            }
            if(board.get_blanket(i,j) == 1){//is hovered
                Draw::set_color(BACKGROUND_LIGHT_YELLOW);
                cout<<" ";
            }
            if(board.get_blanket(i,j) == 2){//is selected num
                cout<<board.get_map(i,j);
            }
            if(board.get_blanket(i,j) == 3){//is selected num and is hovered
                Draw::set_color(BACKGROUND_LIGHT_YELLOW);
                cout<<board.get_map(i,j);
            }
            if(board.get_blanket(i,j) == 4){//is zero
                cout<<".";
            }
            if(board.get_blanket(i,j) == 5){//is zero and hovered
                Draw::set_color(BACKGROUND_LIGHT_YELLOW);
                cout<<".";
            }
            if(board.get_blanket(i,j) == 6){//is flagged
                cout<<"F";
            }
            if(board.get_blanket(i,j) == 7){//is flagged and is hovered
                Draw::set_color(BACKGROUND_LIGHT_YELLOW);
                cout<<"F";
            }
        }
    }
}

void draw_board_skeleton(Board& board){
    for(int i = 0;i < board.get_height();i++){
        for(int j = 0;j < board.get_width();j++){
            Draw::setxy(j+20,i);cout<<board.get_blanket(i,j);
        }
    }
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

void tick(){this_thread::sleep_for(chrono::milliseconds(70));}

void key_handle(int *keyCode){
    thread t1(tick);
    if(GetKeyState(0x57) & 0x8000){// 0x57 = W
        *keyCode = 1;
    }
    if(GetKeyState(0x41) & 0x8000){// 0x41 = A
        *keyCode = 2;
    }
    if(GetKeyState(0x53) & 0x8000){// 0x57 = S
        *keyCode = 3;
    }
    if(GetKeyState(0x44) & 0x8000){// 0x44 = D
        *keyCode = 4;
    }
    if(GetKeyState(VK_RETURN) & 0x8000){
        *keyCode = 5;
    }
    if(GetKeyState(VK_SPACE) & 0x8000){
        *keyCode = 6;
    }
    if(GetKeyState(VK_ESCAPE) & 0x8000){
        *keyCode = 99;
    }
    t1.join();
}

int main(){
    int height = 9,width = 10,bombs = 8;
    run_script("random_api.py",bombs,height,width);
    Board board(height,width);
    board.init_map();
    board.set_bombs_from_file("random_cords.txt");
    int i = 0,j = 0;
    board.set_blanket(0,0,1);
    Draw::show_console_cursor(false);
    draw_board(board);
    bool game_is_over = false;
    while(true){
        int *keyCode = new int;
        *keyCode = 0;
        while(*keyCode == 0){
            key_handle(keyCode);
        }
        if(*keyCode == 99){
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
                draw_board(board);
                Draw::setxy(0,height+1);cout<<"GAME OVER";
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
                draw_board(board);
                Draw::setxy(0,height+1);cout<<"WIN";
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
        draw_board(board);
    }
    Draw::reset_color();
    Draw::setxy(0,20);
    system("pause");
    return 0;
}