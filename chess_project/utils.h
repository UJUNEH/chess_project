#pragma once
/*
 * utils.h
 */
#ifndef UTILS_H_
#define UTILS_H_

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

using namespace std;


// NOTE: YOU CAN ONLY ADD MORE CONSTANTS HERE
// NOTE: FEEL FREE TO UTILIZE BELOW CONSTANTS

static int MAX_BOARD_SIZE = 26;

static map<string, char> piece_to_char = {
    {"King", 'K'},
    {"Queen", 'Q'},
    {"Rook", 'R'},
    {"Bishop", 'B'},
    {"Knight", 'N'},
    {"Pawn", 'P'} 
};
static map<char, string> char_to_piece = {
    {'K', "King"},
    {'Q', "Queen"},
    {'R', "Rook"},
    {'B', "Bishop"},
    {'N', "Knight"},
    {'P', "Pawn"}
};

enum Direction {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT,
                UP_UP_LEFT, UP_UP_RIGHT, DOWN_DOWN_LEFT, DOWN_DOWN_RIGHT,
                LEFT_LEFT_UP, LEFT_LEFT_DOWN, RIGHT_RIGHT_UP, RIGHT_RIGHT_DOWN}; // 16 directions

enum MoveType {MOVE, ATTACK, CHECK, CHECKMATE}; // 4 move types

static map<Direction, pair<int, int>> direction_to_pair = {
    {UP, {0, 1}},
    {DOWN, {0, -1}},
    {LEFT, {-1, 0}},
    {RIGHT, {1, 0}},
    {UP_LEFT, {-1, 1}},
    {UP_RIGHT, {1, 1}},
    {DOWN_LEFT, {-1, -1}},
    {DOWN_RIGHT, {1, -1}},
    {UP_UP_LEFT, {-1, 2}},
    {UP_UP_RIGHT, {1, 2}},
    {DOWN_DOWN_LEFT, {-1, -2}},
    {DOWN_DOWN_RIGHT, {1, -2}},
    {LEFT_LEFT_UP, {-2, 1}},
    {LEFT_LEFT_DOWN, {-2, -1}},
    {RIGHT_RIGHT_UP, {2, 1}},
    {RIGHT_RIGHT_DOWN, {2, -1}}
};

#endif /* UTILS_H_ */