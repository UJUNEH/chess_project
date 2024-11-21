#pragma once
/*
 * game.h
 */
#ifndef CHESSGAME_H_
#define CHESSGAME_H_

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
#include "chessboard.h"
#include "chesspiece.h"

using namespace std;


class Game
{
public:
    void read_file(string filename);
    void solve();
    void solve_print_board();
    void solve_print_possible_moves();
    int solve_check();
    int solve_checkmate();
    int solve_checkmate_in_one_move();

    void open_output_file(string filename="output.txt");


private:
    int problem_id;
    // 0: print board, 1: print possible moves , 2: is it check?
    // 3: is it checkmate?, 4: can it make checkmate in one move?
    string problem_specification;
    ChessBoard* board;
    ofstream ofs;
};

#endif /* CHESSGAME_H_ */