#include "board.h"
#include <iostream>

Map::Map(){
    height = 9;width = 10;
    map = new short[height*width];
}
Map::Map(int _height,int _width){
    height = _height;width = _width;
    map = new short[height*width];
}
void Map::set(int _height,int _width){
    height = _height;width = _width;
    map = new short [height*width];
}
Map::~Map(){
    delete [] map;
}


Board::Board(){
    height = 9;width = 10;
    map.set(height,width);
    blanket.set(height,width);
}
Board::Board(int _height,int _width){
    height = _height;width = _width;
    map.set(height,width);
    blanket.set(height,width);
}

bool Board::is_out_of_bounds(int i,int j){
    if((i < 0 || j < 0) || (i >= height || j>=width)){
        return true;
    }
    return false;
}
bool Board::is_out_of_bounds_raw(int i){
    if((i < 0) || (i >= height*width)){
        return true;
    }
    return false;
}

void Board::init_map(){
    for(int i = 0;i < height*width;i++){
        set_map_raw(i,0);
    }
    init_blanket();
}
void Board::init_blanket(){
    for(int i = 0;i < height*width;i++){
        set_blanket_raw(i,0);
    }
}

int Board::get_from_csv(std::string line, int n){
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

void Board::set_bombs_from_file(std::string f_name){
    short num;
    file.open(f_name);
    std::string line;
    while(getline(file,line)){
        int x = 0,y = 0;
        bool _switch = false;
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

int Board::get_height(){
    return height;
}
int Board::get_width(){
    return width;
}

short Board::get_map(int i,int j){
    if(is_out_of_bounds(i,j)){
        return -101;
    }
    return map.map[i*width+j];
}
short Board::get_map_raw(int i){
    if(is_out_of_bounds_raw(i)){
        return -101;
    }
    return map.map[i];
}
short Board::get_blanket(int i, int j){
    if(is_out_of_bounds(i,j)){
        return -101;
    }
    return blanket.map[i*width+j];
}
void Board::set_map(int i,int j,short n){
    if(!is_out_of_bounds(i,j)){
        map.map[i*width+j] = n;
    }
}
void Board::set_blanket(int i, int j, short n){
    if(!is_out_of_bounds(i,j)){
        blanket.map[i*width+j] = n;
    }
}
void Board::set_map_raw(int i,short n){
    if(!is_out_of_bounds_raw(i)){
        map.map[i] = n;
    }
}
void Board::set_blanket_raw(int i, short n){
    if(!is_out_of_bounds_raw(i)){
        blanket.map[i] = n;
    }
}

void Board::refresh_blanket(){
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

void Board::print_map(){
    for(int i = 0;i < height;i++){
        for(int j = 0;j < width;j++){
            std::cout<<get_map(i,j);
        }
        std::cout<<std::endl;
    }
}

void Board::select(int i,int j){
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

short Board::operator()(int i,int j){
    return map.map[i*height+j];
}

void Board::operator()(int i,int j,short n){
    map.map[i*height+j] = n;
}