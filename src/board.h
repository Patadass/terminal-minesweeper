#include <fstream>
#include <iostream>
#include "draw.h"

#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

struct Map{
private:
    int height,width;
public:
    short* map;
    Map(){
        height = 9;width = 10;
        map = new short[height*width];
    }
    Map(int _height,int _width){
        height = _height;width = _width;
        map = new short[height*width];
    }
    void set(int _height,int _width){
        height = _height;width = _width;
        map = new short [height*width];
    }
    ~Map(){
        delete [] map;
    }
};

class Board{
private:
    int height,width;
    Map map;
    Map blanket;
    std::fstream file;
public:
    Board(){
        height = 9;width = 10;
        map.set(height,width);
        blanket.set(height,width);
    }
    Board(int _height,int _width){
        height = _height;width = _width;
        map.set(height,width);
        blanket.set(height,width);
    }

    bool is_out_of_bounds(int i,int j){
        return (i < 0 || j < 0) || (i >= height || j>=width);
    }
    bool is_out_of_bounds_raw(int i){
        return (i < 0) || (i >= height*width);
    }

    void init_map(){
        for(int i = 0;i < height*width;i++){
            set_map_raw(i,0);
        }
        init_blanket();
    }

    void init_blanket(){
        for(int i = 0;i < height*width;i++){
            set_blanket_raw(i,0);
        }
    }

    void set_mines_from_file(std::string f_name){
        short num;
        file.open(f_name);
        std::string line;
        while(getline(file,line)){
            int x = 0,y = 0;
            x = get_from_csv(line,0);
            y = get_from_csv(line,1);
            set_map(x,y,9);
        }
        file.close();
        for(int i = 0;i < height;i++){
            for(int j = 0;j < width;j++){
                if(get_map(i,j) != 9){
                    continue;
                }
                if(get_map(i-1,j) != 9 && get_map(i-1,j) != -101){
                    num = get_map(i-1,j);
                    set_map(i-1,j, ++num);
                }
                if(get_map(i+1,j) != 9 && get_map(i+1,j) != -101){
                    num = get_map(i+1,j);
                    set_map(i+1,j,++num);
                }
                if(get_map(i,j+1) != 9 && get_map(i,j+1) != -101){
                    num = get_map(i,j+1);
                    set_map(i,j+1,++num);
                }
                if(get_map(i,j-1) != 9 && get_map(i,j-1) != -101){
                    num = get_map(i,j-1);
                    set_map(i,j-1,++num);
                }
                if(get_map(i-1,j+1) != 9 && get_map(i-1,j+1) != -101){
                    num = get_map(i-1,j+1);
                    set_map(i-1,j+1, ++num);
                }
                if(get_map(i-1,j-1) != 9 && get_map(i-1,j-1) != -101){
                    num = get_map(i-1,j-1);
                    set_map(i-1,j-1, ++num);
                }
                if(get_map(i+1,j-1) != 9 && get_map(i+1,j-1) != -101){
                    num = get_map(i+1,j-1);
                    set_map(i+1,j-1,++num);
                }
                if(get_map(i+1,j+1) != 9 && get_map(i+1,j+1) != -101){
                    num = get_map(i+1,j+1);
                    set_map(i+1,j+1,++num);
                }
            }
        }
    }

    int get_from_csv(std::string line,int n){
        int x = 0;
        int j = 0;
        for(int i = 0;i < line.size();i++){
            if(line[i] == ';'){
                j++;
                continue;
            }
            if(j == n){
                x = x * 10 + (line[i] - '0');
            }
        }
        return x;
    }

    int get_height(){
        return height;
    }

    int get_width(){
        return width;
    }

    short get_map(int i,int j){
        if(is_out_of_bounds(i,j)){
            return -101;
        }
        return map.map[i*width+j];
    }

    short get_map_raw(int i){
        if(is_out_of_bounds_raw(i)){
            return -101;
        }
        return map.map[i];
    }
    
    short get_blanket(int i,int j){
        if(is_out_of_bounds(i,j)){
            return -101;
        }
        return blanket.map[i*width+j];
    }
    
    void set_map(int i,int j,short n){
        if(!is_out_of_bounds(i,j)){
            map.map[i*width+j] = n;
        }
    }

    void set_blanket(int i,int j,short n){
        if(!is_out_of_bounds(i,j)){
            blanket.map[i*width+j] = n;
        }
    }

    void set_map_raw(int i,short n){
        if(!is_out_of_bounds_raw(i)){
            map.map[i] = n;
        }
    }
    
    void set_blanket_raw(int i,short n){
        if(!is_out_of_bounds_raw(i)){
            blanket.map[i] = n;
        }
    }

    void refresh_blanket(){
        for(int i = 0;i < height;i++){
            for(int j = 0;j < width;j++){
                if(get_map(i,j) == -1){
                    set_blanket(i, j,4);
                    if(get_map(i-1,j) >= 1 && get_map(i-1,j) <= 8){
                        set_blanket(i-1,j, 2);
                    }
                    if(get_map(i+1,j) >= 1 && get_map(i+1,j) <= 8){
                        set_blanket(i+1,j, 2);
                    }
                    if(get_map(i,j-1) >= 1 && get_map(i,j-1) <= 8){
                        set_blanket(i,j-1, 2);
                    }
                    if(get_map(i,j+1) >= 1 && get_map(i,j+1) <= 8){
                        set_blanket(i,j+1, 2);
                    }
                    if(get_map(i-1,j-1) >= 1 && get_map(i-1,j-1) <= 8){
                        set_blanket(i-1,j-1, 2);
                    }
                    if(get_map(i-1,j+1) >= 1 && get_map(i-1,j+1) <= 8){
                        set_blanket(i-1,j+1, 2);
                    }
                    if(get_map(i+1,j+1) >= 1 && get_map(i+1,j+1) <= 8){
                        set_blanket(i+1,j+1, 2);
                    }
                    if(get_map(i+1,j-1) >= 1 && get_map(i+1,j-1) <= 8){
                        set_blanket(i+1,j-1, 2);
                    }
                }
            }
        }
    }

    void print_map(){
        for(int i = 0;i < height;i++){
            for(int j = 0;j < width;j++){
                printw("%d",get_map(i,j));
            }
            printw("\n");
        }
        refresh();
    }

    void select(int i,int j){
        if(get_map(i,j) != 0 && get_map(i,j) != -1){
            return;
        }
        if(get_map(i-1,j) == 0){
            set_map(i-1,j,-1);
            select(i-1,j);
        }
        if(get_map(i+1,j) == 0){
            set_map(i+1,j,-1);
            select(i+1,j);
        }
        if(get_map(i,j+1) == 0){
            set_map(i,j+1,-1);
            select(i,j+1);
        }
        if(get_map(i,j-1) == 0){
            set_map(i,j-1,-1);
            select(i,j-1);
        }
    }

    short operator()(int i,int j){
        return map.map[i*height+j];
    }

    void operator()(int i,int j,short n){
        map.map[i*height+j] = n;
    }

};

#endif //MINESWEEPER_BOARD_H
