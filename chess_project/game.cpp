#include "game.h"

using namespace std;


void Game::read_file(string filename)
{
    ifstream ifs(filename);
    if (!ifs)
    {
        cerr << "Error: file open failed" << endl;
        exit(1);
    }
    
    ifs >> problem_id;
    ifs.ignore();
    getline(ifs, problem_specification);

    cout << "problem_id: [" << problem_id << "]" << endl;
    cout << "problem_specification: [" << problem_specification << "]" << endl;

    // '8 8'이면 숫자 받아서 저장하고 N, M에 저장 ChessBoard* board로 선언되어 있음.

    int col, row;
    ifs >> row; // row
    ifs >> col; // col
    ifs.ignore();
    string row_line;
    string color;
    string type;
    int flag = 0; // flag 0은 기본, 2는 2칸 나아갈 수 있도록 알려주는 표지.

    board = new ChessBoard(col, row);
    for(int i=0; i<row; i++){
        getline(ifs, row_line);
        for(int j=0; j<col; j++){
            if(isalpha(row_line[j])){
                //cout << "f" << endl;
                if(isupper(row_line[j])){
                    color = "White";
                    type = char_to_piece[row_line[j]];
                    if((i==(row-2)) && (type == "Pawn")){
                        flag = 2; 
                    }
                }
                else if(islower(row_line[j])){
                    color = "Black";
                    type = char_to_piece[toupper(row_line[j])];
                    if((i==1) && (type == "Pawn")){
                        flag = 2;
                    }
                }
                // cout << flag << " " << color << type << endl;
                (*board).set_tile(i, j, create_piece(color, type, i, j, flag));
                flag = 0;
            }
        }
    }
}

void Game::open_output_file(string filename)
{
    ofs.open(filename);
    if (!ofs)
    {
        cerr << "Error: file open failed" << endl;
        exit(1);
    }
}

void Game::solve()
{
    switch (problem_id)
    {
    case 0:
        solve_print_board();
        break;
    case 1:
        solve_print_possible_moves();
        break;
    case 2:
        solve_check();
        break;
    case 3:
        solve_checkmate();
        break;
    case 4:
        solve_checkmate_in_one_move();
        break;
    default:
        cout << "Invalid problem_id" << endl;
        break;
    }
    return;
}

void Game::solve_print_board()
{
    ofs << *board;
}

void Game::solve_print_possible_moves()
{
    char target_piece = problem_specification[0];
    char target_col = problem_specification[1];
    string target_row;
    if (problem_specification.length() == 3){
        target_row[0] = '0';
        target_row[1] = problem_specification[2];
    }
    else if (problem_specification.length() == 4){
        target_row[0] = problem_specification[2];
        target_row[1] = problem_specification[3];
    }
    int t_col = int(target_col - 'a');
    int t_row = int(target_row[0] - '0') * 10 + int(target_row[1] - '0');
    t_row = (*board).get_height() - t_row;

    ChessPiece* target = (*board).get_tiles(t_row, t_col);
   
    string color, type;
    if(isupper(target_piece)){
        color = "White";
        type = char_to_piece[target_piece];
    }
    else {
        color = "Black";
        type = char_to_piece[toupper(target_piece)];
    } 
    //cout << type << endl;

    //vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();

    if(type == "Pawn"){ // Pawn일 때 판단
        vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();
        int sign = 0;
        for(int i=0; i<v.size(); i++){
            tuple<MoveType, Direction, int> tup = v[i];
            pair <int, int> p = direction_to_pair[get<1>(tup)];
            int number = get<2>(tup);

            int temp_row = t_row;
            int temp_col = t_col;
            temp_row = temp_row - p.second*number;
            temp_col = temp_col + p.first*number;
            //일단 영역 안에 들어오는지 판단
            if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                if(get<0>(tup) == MOVE){
                    if(number == 1){
                        if((*board).get_tiles(temp_row, temp_col)==nullptr){
                            (*board).set_trait(temp_row, temp_col, 1);
                        }
                        else if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                            (*board).set_trait(temp_row, temp_col, 0);
                            sign = 1;
                        }
                    }
                    else if(number==2){
                        if(sign==0){
                            if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                (*board).set_trait(temp_row, temp_col, 1);
                            }
                            else if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                                (*board).set_trait(temp_row, temp_col, 0);
                            }
                        }
                        else if(sign==1){
                            (*board).set_trait(temp_row, temp_col, 0);
                        }
                    }
                }
                else if(get<0>(tup) == ATTACK){
                    if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                        if((*board).get_tiles(temp_row, temp_col)->get_color() != color){
                            (*board).set_trait(temp_row, temp_col, 2);
                        }
                    }
                }
            }
        }
    }
    else if(type == "Knight"){ // Knight일 때 판단
        vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();
        for(int i=0; i<v.size(); i++){
            
            tuple<MoveType, Direction, int> tup = v[i];
            pair <int, int> p = direction_to_pair[get<1>(tup)];
            int number = get<2>(tup);

            int temp_row = t_row;
            int temp_col = t_col;
            temp_row = temp_row - p.second*number;
            temp_col = temp_col + p.first*number;
            // cout << "ddd" <<endl;
            //cout << temp_row << temp_col << endl;
            //cout << t_row << t_col <<endl;
            if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                // if문 안은 tile에 기물이 없는 경우 -> o를 출력해야 함.
                if((*board).get_tiles(temp_row, temp_col) == nullptr){
                    (*board).set_trait(temp_row, temp_col, 1);
                    //cout << (*board).get_trait(temp_row, temp_col) << endl;
                }
                // else문 안은 tile에 기물이 있는 경우 -> 존재하고 있는 기물의 색깔에 따라 
                // 다른 색깔이라면 x, 같은 색깔이라면 아무것도 출력하면 안 됨. 
                else{
                    if((*board).get_tiles(temp_row, temp_col)->get_color() == color){
                       (*board).set_trait(temp_row, temp_col, 0); 
                       //cout << (*board).get_trait(temp_row, temp_col) << endl;
                    }
                    else{
                        (*board).set_trait(temp_row, temp_col, 2);
                        //cout << (*board).get_trait(temp_row, temp_col) << endl;
                    }
                }
            }
            else{
                //cout << "conti"<<endl;
                continue;
            }
        }
    }
    // 밑의 else 문은 bishop, rook, queen, king에 관한 것.
    else{
        vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();
        map <Direction, bool> stopsign; // 계속 이동하다가 누구를 만나면 stop 해야 함. 1이면 stop
        for(auto direction : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_DOWN_RIGHT}){
            stopsign[direction] = 0;
        }
        for(int i=0; i<v.size(); i++){
            tuple<MoveType, Direction, int> tup = v[i];
            pair <int, int> p = direction_to_pair[get<1>(tup)];
            int number = get<2>(tup);

            int temp_row = t_row;
            int temp_col = t_col;
            temp_row = temp_row - p.second*number;
            temp_col = temp_col + p.first*number;
            //일단 영역 안에 들어오는지 판단
            if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                if(stopsign[get<1>(tup)] == 0){
                    if((*board).get_tiles(temp_row, temp_col)==nullptr){
                        (*board).set_trait(temp_row, temp_col, 1);
                    }
                    else if((*board).get_tiles(temp_row, temp_col)->get_color() != color){
                        (*board).set_trait(temp_row, temp_col, 2);
                        stopsign[get<1>(tup)] = 1;
                    }
                    else {
                        stopsign[get<1>(tup)] = 1;
                    }
                }
            }
        }
    }
    for(int i=0; i<(*board).get_height(); i++){
        for(int j=0; j<(*board).get_width(); j++){
            if(i==t_row && j==t_col){
                ofs << target_piece;
                //cout << target_piece;
            }
            else if((*board).get_trait(i, j)==0){
                ofs << ".";
                //cout << ".";
            }
            else if((*board).get_trait(i, j)==1){
                ofs << "o";
                //cout << "o";
            }
            else if((*board).get_trait(i, j)==2){
                ofs << "x";
                //cout << "x";
            }
        }
        ofs << endl;
        //cout << endl;
    }
}

int Game::solve_check()
{
    // EXAMPLE: "Black king is in check"
    //          "White king is not in check"
    string color;
    if (problem_specification[0] == 'b'){
        color = "Black";
    }
    else if(problem_specification[0] == 'w'){
        color = "White";
    }
    // possible move 판단법을 이용하여 king자리에 x 표시가 있으면 check 출력
    for (int m=0; m<(*board).get_height(); m++){
        for (int n=0; n<(*board).get_width(); n++){
            if((*board).get_tiles(m, n)!=nullptr){
                if((*board).get_tiles(m, n)->get_color() != color){
                        ChessPiece* target = (*board).get_tiles(m, n);
                        string type = (*board).get_tiles(m, n)->get_type();
                        if(type == "Pawn"){ // Pawn일 때 판단
                            vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();
                            int sign = 0;
                            for(int i=0; i<v.size(); i++){
                                tuple<MoveType, Direction, int> tup = v[i];
                                pair <int, int> p = direction_to_pair[get<1>(tup)];
                                int number = get<2>(tup);

                                int temp_row = m;
                                int temp_col = n;
                                temp_row = temp_row - p.second*number;
                                temp_col = temp_col + p.first*number;
                                //일단 영역 안에 들어오는지 판단
                                if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                                    if(get<0>(tup) == MOVE){
                                        if(number == 1){
                                            if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                                (*board).set_trait(temp_row, temp_col, 1);
                                            }
                                            else if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                                                sign = 1;
                                            }
                                        }
                                        else if(number==2){
                                            if(sign==0){
                                                if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                                    (*board).set_trait(temp_row, temp_col, 1);
                                                }
                                                else if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                                                    (*board).set_trait(temp_row, temp_col, 0);
                                                }
                                            }
                                            else if(sign==1){
                                                (*board).set_trait(temp_row, temp_col, 0);
                                            }
                                        }
                                    }
                                    else if(get<0>(tup) == ATTACK){
                                        if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                                            if((*board).get_tiles(temp_row, temp_col)->get_color() == color){
                                                (*board).set_trait(temp_row, temp_col, 2);
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        else if(type == "Knight"){ // Knight일 때 판단
                            vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();
                            for(int i=0; i<v.size(); i++){
                                
                                tuple<MoveType, Direction, int> tup = v[i];
                                pair <int, int> p = direction_to_pair[get<1>(tup)];
                                int number = get<2>(tup);

                                int temp_row = m;
                                int temp_col = n;
                                temp_row = temp_row - p.second*number;
                                temp_col = temp_col + p.first*number;
                                // cout << "ddd" <<endl;
                                //cout << temp_row << temp_col << endl;
                                //cout << t_row << t_col <<endl;
                                if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                                    // if문 안은 tile에 기물이 없는 경우 -> o를 출력해야 함.
                                    if((*board).get_tiles(temp_row, temp_col) == nullptr){
                                        (*board).set_trait(temp_row, temp_col, 1);
                                        //cout << (*board).get_trait(temp_row, temp_col) << endl;
                                    }
                                    // else문 안은 tile에 기물이 있는 경우 -> 존재하고 있는 기물의 색깔에 따라 
                                    // 다른 색깔이라면 x, 같은 색깔이라면 아무것도 출력하면 안 됨. 
                                    else{
                                        if((*board).get_tiles(temp_row, temp_col)->get_color() != color){
                                        (*board).set_trait(temp_row, temp_col, 0); 
                                        //cout << (*board).get_trait(temp_row, temp_col) << endl;
                                        }
                                        else{
                                            (*board).set_trait(temp_row, temp_col, 2);
                                            //cout << (*board).get_trait(temp_row, temp_col) << endl;
                                        }
                                    }
                                }
                                else{
                                    //cout << "conti"<<endl;
                                    continue;
                                }
                            }
                        }

                        // 밑의 else 문은 bishop, rook, queen, king에 관한 것.
                        else{
                            vector<tuple<MoveType, Direction, int>> v = target->get_possible_moves();
                            map <Direction, bool> stopsign; // 계속 이동하다가 누구를 만나면 stop 해야 함. 1이면 stop
                            for(auto direction : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_DOWN_RIGHT}){
                                stopsign[direction] = 0;
                            }
                            for(int i=0; i<v.size(); i++){
                                tuple<MoveType, Direction, int> tup = v[i];
                                pair <int, int> p = direction_to_pair[get<1>(tup)];
                                int number = get<2>(tup);
                                int temp_row = m;
                                int temp_col = n;
                                temp_row = temp_row - p.second*number;
                                temp_col = temp_col + p.first*number;
                                //일단 영역 안에 들어오는지 판단
                                if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                                    if(stopsign[get<1>(tup)] == 0){
                                        if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                            (*board).set_trait(temp_row, temp_col, 1);
                                        }
                                        else if((*board).get_tiles(temp_row, temp_col)->get_color() == color){
                                            (*board).set_trait(temp_row, temp_col, 2);
                                            stopsign[get<1>(tup)] = 1;
                                        }
                                        else {
                                            stopsign[get<1>(tup)] = 1;
                                        }
                                    }
                                }
                            }
                        }
                }
            }
        }   
    }
    if(problem_id==2){
        for(int i=0; i<(*board).get_height(); i++){
            for(int j=0; j<(*board).get_width(); j++){
                if((*board).get_tiles(i, j) == nullptr) continue;
                else if (((*board).get_tiles(i, j)->get_color() == color ) && ((*board).get_tiles(i, j)->get_type() == "King")){
                    if ((*board).get_trait(i, j) == 2){
                        ofs << color << " " << "king is in check";
                        // cout << color << " " << "king is in check" << endl;
                    }
                    else{
                        ofs << color << " " << "king is not in check";
                        // cout << color << " " << "king is not in check" << endl;
                    }
                }
            }
        }
    }
    else if(problem_id==3 || problem_id==4){
        for(int i=0; i<(*board).get_height(); i++){
            for(int j=0; j<(*board).get_width(); j++){
                if((*board).get_tiles(i, j) == nullptr) continue;
                else if (((*board).get_tiles(i, j)->get_color() == color ) && ((*board).get_tiles(i, j)->get_type() == "King")){
                    if ((*board).get_trait(i, j) == 2){
                        return 1; // check인 경우에 1 반환
                    }
                    else{
                        return 0; // check가 아닌 경우에 0 반환
                    }
                }
            }
        }
    }
    return 0;
}

int Game::solve_checkmate()
{
    // EXAMPLE: "Black king is checkmated"
    //          "White king is not checkmated"
    string color;
    if (problem_specification[0] == 'b'){
        color = "Black";
    }
    else if(problem_specification[0] == 'w'){
        color = "White";
    }
    // 새로운 객체 생성해서 deep copy 구현
    ChessBoard default_board_ptr(*board);  

    if (!solve_check()) { // 스테일메이트 제거 코드
        if(problem_id == 3){
            ofs << color << " " << "king is not checkmated"; 
        }
        return 0;
    }
    else{
    for(int i=0; i<(default_board_ptr).get_height(); i++){
        for(int j=0; j<(default_board_ptr).get_width(); j++){
            if((default_board_ptr).get_tiles(i, j) != nullptr && (default_board_ptr).get_tiles(i, j)->get_color() == color){
                // 실제로 말을 움직이고 그거를 board 변수에 반영해 놓고 solve_check() 불러와서 판단한다.
                if((default_board_ptr).get_tiles(i, j)->get_type() == "Pawn"){ //Pawn
                    vector<tuple<MoveType, Direction, int>> v = (default_board_ptr).get_tiles(i, j)->get_possible_moves();
                    int sign = 0; // stop을 알려주는 표지
                    for(int vector_index=0; vector_index<v.size(); vector_index++){
                        tuple<MoveType, Direction, int> tup = v[vector_index];
                        pair <int, int> p = direction_to_pair[get<1>(tup)];
                        int number = get<2>(tup);

                        int temp_row = i;
                        int temp_col = j;
                        temp_row = temp_row - p.second*number;
                        temp_col = temp_col + p.first*number;
                        //일단 영역 안에 들어오는지 판단
                        if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                            if(get<0>(tup) == MOVE){
                                if(number == 1){
                                    if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                        (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                        (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                        (*board).set_tile(i, j, nullptr);
                                    }
                                    else {
                                        sign = 1;
                                        continue;
                                    }
                                }
                                else if(number==2){
                                    if(sign==0){
                                        if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                            (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                            (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                            (*board).set_tile(i, j, nullptr);
                                        }
                                        else continue;
                                    }
                                    else continue;
                                }
                            }
                            else if(get<0>(tup) == ATTACK){
                                if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                                    if((*board).get_tiles(temp_row, temp_col)->get_color() != color){
                                        (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                        (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                        (*board).set_tile(i, j, nullptr);
                                    }
                                    else continue;
                                }
                                else continue;
                            }
                            if (!solve_check()){
                                (*board)= ChessBoard(default_board_ptr);
                                if(problem_id == 3){
                                    ofs << color << " " << "king is not checkmated";
                                }
                                return 0;
                            }
                            (*board)= ChessBoard(default_board_ptr);
                        }
                    }
                }

                else if((default_board_ptr).get_tiles(i, j)->get_type() == "Knight"){ // Knight
                    vector<tuple<MoveType, Direction, int>> v = (default_board_ptr).get_tiles(i, j)->get_possible_moves();
                    for(int vector_index=0; vector_index<v.size(); vector_index++){
                        tuple<MoveType, Direction, int> tup = v[vector_index];
                        pair <int, int> p = direction_to_pair[get<1>(tup)];
                        int number = get<2>(tup);

                        int temp_row = i;
                        int temp_col = j;
                        temp_row = temp_row - p.second*number;
                        temp_col = temp_col + p.first*number;

                        if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                            // if문 안은 tile에 기물이 없는 경우 -> 이동 가능
                            if((default_board_ptr).get_tiles(temp_row, temp_col) == nullptr){
                                (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                (*board).set_tile(i, j, nullptr);
                            }
                            // else문 안은 tile에 기물이 있는 경우 -> 존재하고 있는 기물의 색깔에 따라 
                            // 다른 색깔이라면 먹은 다음 이동 가능, 같은 색깔이라면 이동 불가 => for문 처음으로 돌아가기
                            else{
                                if((default_board_ptr).get_tiles(temp_row, temp_col)->get_color() == color){
                                    (*board)= ChessBoard(default_board_ptr);
                                    continue;
                                }   
                                else{
                                    (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                    (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                    (*board).set_tile(i, j, nullptr);
                                }
                            }
                            if(!solve_check()){
                                (*board)= ChessBoard(default_board_ptr); // 무의미한 행동이긴 하지만, 이해를 위해 적음
                                if(problem_id == 3){
                                    ofs << color << " " << "king is not checkmated";
                                }
                                return 0;
                            }
                            else if(solve_check()){
                                (*board) = ChessBoard(default_board_ptr);
                                continue;
                            }
                        }
                    }
                }

                else{ // Pawn, Knight 제외하고 나머지
                    vector<tuple<MoveType, Direction, int>> v = (default_board_ptr).get_tiles(i, j)->get_possible_moves();
                    map <Direction, bool> stopsign; // 계속 이동하다가 누구를 만나면 stop 해야 함. 1이면 stop
                    for(auto direction : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_DOWN_RIGHT}){
                        stopsign[direction] = 0;
                    }
                    for(int vector_index=0; vector_index<v.size(); vector_index++){
                        tuple<MoveType, Direction, int> tup = v[vector_index];
                        pair <int, int> p = direction_to_pair[get<1>(tup)];
                        int number = get<2>(tup);
                        int temp_row = i;
                        int temp_col = j;
                        temp_row = temp_row - p.second*number;
                        temp_col = temp_col + p.first*number;
                        //일단 영역 안에 들어오는지 판단
                        if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                            if(stopsign[get<1>(tup)] == 0){
                                if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                    (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                    (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                    (*board).set_tile(i, j, nullptr);
                                }
                                else if((*board).get_tiles(temp_row, temp_col)->get_color() != color){
                                    (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                    (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                    (*board).set_tile(i, j, nullptr);
                                    stopsign[get<1>(tup)] = 1;
                                }
                                else {
                                    stopsign[get<1>(tup)] = 1;
                                    continue;
                                }
                            }
                            else continue;

                            if(!solve_check()){
                                (*board) = default_board_ptr; 
                                if(problem_id == 3){
                                    ofs << color << " " << "king is not checkmated";
                                }
                                return 0;
                            }
                            else if(solve_check()){
                                (*board) = default_board_ptr;
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }
    if(problem_id == 3){ 
        ofs << color << " " << "king is checkmated";
    }
    return 1;
    }
}

int Game::solve_checkmate_in_one_move()
{
    // EXAMPLE: "Black king is checkmated in one move"
    //          "White king is not checkmated in one move"
    string color;
    if (problem_specification[0] == 'b'){
        color = "Black";
    }
    else if(problem_specification[0] == 'w'){
        color = "White";
    }
    ChessBoard default_board_ptr(*board); 

    for(int i=0; i<(default_board_ptr).get_height(); i++){
            for(int j=0; j<(default_board_ptr).get_width(); j++){
                if((default_board_ptr).get_tiles(i, j) != nullptr && (default_board_ptr).get_tiles(i, j)->get_color() != color){
                    // 실제로 말을 움직이고 그거를 board 변수에 반영해 놓고 solve_checkmate() 불러와서 판단한다.
                    
                    if((default_board_ptr).get_tiles(i, j)->get_type() == "Pawn"){ //Pawn
                        vector<tuple<MoveType, Direction, int>> v = (default_board_ptr).get_tiles(i, j)->get_possible_moves();
                        int sign = 0; // stop을 알려주는 표지
                        for(int vector_index=0; vector_index<v.size(); vector_index++){
                            tuple<MoveType, Direction, int> tup = v[vector_index];
                            pair <int, int> p = direction_to_pair[get<1>(tup)];
                            int number = get<2>(tup);

                            int temp_row = i;
                            int temp_col = j;
                            temp_row = temp_row - p.second*number;
                            temp_col = temp_col + p.first*number;
                            //일단 영역 안에 들어오는지 판단
                            if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                                if(get<0>(tup) == MOVE){
                                    if(number == 1){
                                        if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                            (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                            (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                            (*board).set_tile(i, j, nullptr);
                                        }
                                        else {
                                            sign = 1;
                                            continue;
                                        }
                                    }
                                    else if(number==2){
                                        if(sign==0){
                                            if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                                (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                                (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                                (*board).set_tile(i, j, nullptr);
                                            }
                                            else continue;
                                        }
                                        else continue;
                                    }
                                }
                                else if(get<0>(tup) == ATTACK){
                                    if((*board).get_tiles(temp_row, temp_col)!=nullptr){
                                        if((*board).get_tiles(temp_row, temp_col)->get_color() == color){
                                            (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                            (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                            (*board).set_tile(i, j, nullptr);
                                        }
                                        else continue;
                                    }
                                    else continue;
                                }
                                if (solve_checkmate()){
                                    (*board)= ChessBoard(default_board_ptr);
                                    ofs << color << " " << "king is checkmated in one move";
                                    return 1;
                                }
                                (*board)= ChessBoard(default_board_ptr);
                            }
                        }
                    }

                    else if((default_board_ptr).get_tiles(i, j)->get_type() == "Knight"){ // Knight
                        vector<tuple<MoveType, Direction, int>> v = (default_board_ptr).get_tiles(i, j)->get_possible_moves();
                        for(int vector_index=0; vector_index<v.size(); vector_index++){
                            tuple<MoveType, Direction, int> tup = v[vector_index];
                            pair <int, int> p = direction_to_pair[get<1>(tup)];
                            int number = get<2>(tup);

                            int temp_row = i;
                            int temp_col = j;
                            temp_row = temp_row - p.second*number;
                            temp_col = temp_col + p.first*number;

                            if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                                // if문 안은 tile에 기물이 없는 경우 -> 이동 가능
                                if((default_board_ptr).get_tiles(temp_row, temp_col) == nullptr){
                                    (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                    (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                    (*board).set_tile(i, j, nullptr);
                                }
                                // else문 안은 tile에 기물이 있는 경우 -> 존재하고 있는 기물의 색깔에 따라 
                                // 다른 색깔이라면 먹은 다음 이동 가능, 같은 색깔이라면 이동 불가 => for문 처음으로 돌아가기
                                else{
                                    if((default_board_ptr).get_tiles(temp_row, temp_col)->get_color() != color){
                                        (*board)= ChessBoard(default_board_ptr);
                                        continue;
                                    }   
                                    else{
                                        (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                        (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                        (*board).set_tile(i, j, nullptr);
                                    }
                                }
                                if(solve_checkmate()){
                                    (*board)= ChessBoard(default_board_ptr); // 무의미한 행동이긴 하지만, 이해를 위해 적음
                                    ofs << color << " " << "king is checkmated in one move";
                                    return 1;
                                }
                                else if(!solve_checkmate()){
                                    (*board) = ChessBoard(default_board_ptr);
                                    continue;
                                }
                            }
                        }
                    }

                    else{ // Pawn, Knight 제외하고 나머지
                        
                        vector<tuple<MoveType, Direction, int>> v = (default_board_ptr).get_tiles(i, j)->get_possible_moves();
                        map <Direction, bool> stopsign; // 계속 이동하다가 누구를 만나면 stop 해야 함. 1이면 stop
                        for(auto direction : {UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_DOWN_RIGHT}){
                            stopsign[direction] = 0;
                        }
                        for(int vector_index=0; vector_index<v.size(); vector_index++){
                            
                            tuple<MoveType, Direction, int> tup = v[vector_index];
                            pair <int, int> p = direction_to_pair[get<1>(tup)];
                            int number = get<2>(tup);
                            int temp_row = i;
                            int temp_col = j;
                            temp_row = temp_row - p.second*number;
                            temp_col = temp_col + p.first*number;
                            //일단 영역 안에 들어오는지 판단
                            if((temp_row>=0) && (temp_row<(*board).get_height()) && (temp_col>=0) && (temp_col<(*board).get_width())){
                                if(stopsign[get<1>(tup)] == 0){
                                    if((*board).get_tiles(temp_row, temp_col)==nullptr){
                                        (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                        (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                        (*board).set_tile(i, j, nullptr);
                                    }
                                    else if((*board).get_tiles(temp_row, temp_col)->get_color() == color){
                                        (*board).get_tiles(i, j)->move(temp_row, temp_col);
                                        (*board).set_tile(temp_row, temp_col, (*board).get_tiles(i, j));
                                        (*board).set_tile(i, j, nullptr);
                                        stopsign[get<1>(tup)] = 1;
                                    }
                                    else {
                                        stopsign[get<1>(tup)] = 1;
                                        continue;
                                    }
                                }
                                else continue;
                                
                                
                                if(solve_checkmate()){
                                    (*board) = default_board_ptr; 
                                    ofs << color << " " << "king is checkmated in one move";
                                    return 1;
                                }
                                else if(!solve_checkmate()){
                                    (*board) = default_board_ptr;
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
        }
        ofs << color << " " << "king is not checkmated in one move";
        return 0;
}
      
    
