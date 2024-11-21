#pragma once
/*
 * board.h
 */
#ifndef CHESSPIECE_H_
#define CHESSPIECE_H_

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

using namespace std;

class ChessPiece
{
public:
    ChessPiece(string, string, int, int, int=0);
    ChessPiece(const ChessPiece& other);
    friend ostream& operator<<(ostream& os, const ChessPiece& piece);

    string get_color() { return color; }
    string get_type() { return type; }

    int get_x() { return x; }
    int get_y() { return y; }
    int get_flag() { return flag; }

    void set_x(int x) { this->x = x; }
    void set_y(int y) { this->y = y; }
    void set_flag(int flag) { this->flag = flag; }

    virtual void move(int x, int y) { this->x = x; this->y = y; };

    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves() = 0; 

protected:
    string color;
    string type;
    int x;
    int y;
    int flag;
};

ChessPiece* create_piece(string color, string type, int x, int y, int flag=0);


class King : public ChessPiece
{
public:
    King(string color, int x, int y, int flag) : ChessPiece(color, "King", x, y, flag) {};
    virtual void move(int x, int y) { this->x = x; this->y = y; };
    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        // vector 안에 있는 tuple은 몇 번 행동을 할 수 있는지 정해주는 것.
        vector<tuple<MoveType, Direction, int>> v;
        for (auto d : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT}){
            v.push_back(make_tuple(MOVE, d, 1));
        }
        return v;
    }
};

class Queen : public ChessPiece
{
public:
    Queen(string color, int x, int y, int flag) : ChessPiece(color, "Queen", x, y, flag) {};
    virtual void move(int x, int y) { this->x = x; this->y = y; };
    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> v;
        for (auto d : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT}){
            for(int i=1; i<MAX_BOARD_SIZE; i++){
                v.push_back(make_tuple(MOVE, d, i));
            }
        }
        return v;
    }
};

class Rook : public ChessPiece
{
public:
    Rook(string color, int x, int y, int flag) : ChessPiece(color, "Rook", x, y, flag) {};
    virtual void move(int x, int y) { this->x = x; this->y = y; };
    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> v;
        for(auto d : {UP, DOWN, LEFT, RIGHT}){
            for(int i=1; i<MAX_BOARD_SIZE; i++){
                v.push_back(make_tuple(MOVE, d, i));
            }
        }
        return v;
    }
};

class Bishop : public ChessPiece
{
public:
    Bishop(string color, int x, int y, int flag) : ChessPiece(color, "Bishop", x, y, flag) {};
    virtual void move(int x, int y) { this->x = x; this->y = y; };
    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> v;            
        for(auto d : {UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT}){
            for(int i=1; i<MAX_BOARD_SIZE; i++){
                v.push_back(make_tuple(MOVE, d, i));
            }
        }
        return v;            
    }
};

class Knight : public ChessPiece
{
public:
    Knight(string color, int x, int y, int flag) : ChessPiece(color, "Knight", x, y, flag) {};
    virtual void move(int x, int y) { this->x = x; this->y = y; };
    virtual vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> v; 
        for(auto d : {UP_UP_LEFT, UP_UP_RIGHT, DOWN_DOWN_LEFT, DOWN_DOWN_RIGHT, LEFT_LEFT_UP, LEFT_LEFT_DOWN, RIGHT_RIGHT_UP, RIGHT_RIGHT_DOWN}){
            v.push_back(make_tuple(MOVE, d, 1));
        }
        return v;
    }
};

class Pawn : public ChessPiece
{
public:
    Pawn(string color, int x, int y, int flag) : ChessPiece(color, "Pawn", x, y, flag) {};
    virtual void move(int x, int y) { this->x = x; this->y = y; };
    virtual  vector<tuple<MoveType, Direction, int>> get_possible_moves(){
        vector<tuple<MoveType, Direction, int>> v; 
        if(color == "White"){
            v.push_back(make_tuple(ATTACK, UP_LEFT, 1));
            v.push_back(make_tuple(ATTACK, UP_RIGHT, 1));
            if(flag == 2){
                v.push_back(make_tuple(MOVE, UP, 1));
                v.push_back(make_tuple(MOVE, UP, 2));
            }
            else if(flag == 0){
                v.push_back(make_tuple(MOVE, UP, 1));
            }
        }
        else if(color == "Black"){
            v.push_back(make_tuple(ATTACK, DOWN_LEFT, 1));
            v.push_back(make_tuple(ATTACK, DOWN_RIGHT, 1));
            //cout << flag << endl;
            if(flag == 2){
                v.push_back(make_tuple(MOVE, DOWN, 1));
                v.push_back(make_tuple(MOVE, DOWN, 2));
            }
            else if(flag == 0){
                v.push_back(make_tuple(MOVE, DOWN, 1));
            }            
        }
        return v;
    }
};

#endif /* CHESSPIECE_H_ */