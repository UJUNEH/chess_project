#pragma once

#ifndef CHESSBOARD_H_
#define CHESSBOARD_H_

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cctype>

#include "utils.h"
#include "chesspiece.h"

using namespace std;


class ChessBoard
{
public:
    void initialize_board();
    
    ChessBoard(int width, int height): width(width), height(height) { initialize_board(); }

    ChessBoard& operator=(const ChessBoard& other);
    
    friend ostream& operator<<(ostream& os, const ChessBoard& board);
    ChessPiece* operator[](string s);

    ChessBoard(const ChessBoard& other);

    void set_tile(int i, int j, ChessPiece* p);

    void set_trait(int i, int j, int num){ // trait[i][j] (. 0) (o 1) (x 2)
        trait[i][j] = num; 
    }

    int get_height(){
        return height;
    }
    int get_width(){
        return width;
    }
    int get_trait(int i, int j){
        return trait[i][j];
    }

    ChessPiece* get_tiles(int i , int j) const{
        return tiles[i][j];
    }

    ~ChessBoard();

private: 
    int width, height;
    int** trait; // o가 들어가야할지(1), x가 들어가야할지(2), 아니면 아무것도 안들어가야할지(0) 결정
    ChessPiece*** tiles;
    set<ChessPiece*> white_pieces;
    set<ChessPiece*> black_pieces;
};



#endif /* CHESSBOARD_H_ */