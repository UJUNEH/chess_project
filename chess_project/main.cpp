#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cctype>

#include "chessboard.h"
#include "chesspiece.h"
#include "game.h"

using namespace std;


int main(int argc, char* argv[])
{
    
    string input_file = "input.txt";
    string output_file = "output.txt";

    if(argc == 1) // no args
    {   
        input_file = "input.txt";
        output_file = "output.txt";
    } 
    else if(argc == 2)
    {
        input_file = "../inputs/input" + string(argv[1]) + ".txt";
        output_file = "../outputs/output" + string(argv[1]) + ".txt";
    }
    
    cout << "reading from " << input_file << endl;
    cout << "writing to " << output_file << endl;

    Game game;
    
    game.read_file(input_file);
    game.open_output_file(output_file);

    game.solve();

    return 0;
}

