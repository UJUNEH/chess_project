#include "chessboard.h"

using namespace std;


void ChessBoard::initialize_board()
{
    // constructor로 width와 height는 정해진 상태
    tiles = new ChessPiece**[height];
    trait = new int*[height];
    for(int i=0; i < height; i++){
        tiles[i] = new ChessPiece*[width]; 
        trait[i] = new int[width];
        for(int j=0; j<width; j++){
            tiles[i][j] = nullptr;
            trait[i][j] = 0; // o x 판단하는 멤버 변수 초기화
        }
    }
}

ostream& operator<<(ostream& os, const ChessBoard& board)
{
    for (int i = 0; i < board.height; i++){
        for (int j = 0; j < board.width; j++){
            if (board.tiles[i][j] == nullptr) {
                os << '.';
            }
            else{
                os << *board.tiles[i][j];
            }
        }
        os << endl;
    }
    return os;
}


ChessBoard& ChessBoard::operator=(const ChessBoard& other)
{
    this->height = other.height;
    this->width = other.width;
    this->tiles = new ChessPiece**[height];
    this->trait = new int*[height];
    for(int i=0; i<height; i++){
        this->tiles[i] = new ChessPiece*[width];
        this->trait[i] = new int[width];
        for(int j=0; j<width; j++){
            if(other.get_tiles(i, j) != nullptr){
                this->tiles[i][j] = create_piece(other.get_tiles(i, j)->get_color(), other.get_tiles(i, j)->get_type(), other.get_tiles(i, j)->get_x(), other.get_tiles(i, j)->get_y(), other.get_tiles(i, j)->get_flag());
            }
            else this->tiles[i][j] = nullptr;
            this->trait[i][j] = other.trait[i][j];
        }
    }
    this->white_pieces = other.white_pieces;
    this->black_pieces = other.black_pieces;
    
    return *this;
}

ChessBoard::ChessBoard(const ChessBoard& other)
{
    this->height = other.height;
    this->width = other.width;
    this->tiles = new ChessPiece**[height];
    this->trait = new int*[height];
    for(int i=0; i<height; i++){
        this->tiles[i] = new ChessPiece*[width];
        this->trait[i] = new int[width];
        for(int j=0; j<width; j++){
            if(other.get_tiles(i, j) != nullptr){
                this->tiles[i][j] = create_piece(other.get_tiles(i, j)->get_color(), other.get_tiles(i, j)->get_type(), other.get_tiles(i, j)->get_x(), other.get_tiles(i, j)->get_y(), other.get_tiles(i, j)->get_flag());
            }
            else this->tiles[i][j] = nullptr;
            trait[i][j] = other.trait[i][j];
        }
    }
    this->white_pieces = other.white_pieces;
    this->black_pieces = other.black_pieces;
}

void ChessBoard::set_tile(int i, int j, ChessPiece* p){
    if(tiles[i][j] != nullptr){
        delete tiles[i][j];
        tiles[i][j] = nullptr;
    }
    if(p == nullptr){
        tiles[i][j] = nullptr;
    }
    else{
        tiles[i][j] = create_piece(p->get_color(), p->get_type(), i, j, p->get_flag());
    }
}

ChessBoard::~ChessBoard()
{
    for (int i=0; i<this->get_width(); i++)
    {
        delete[] trait[i];
    }
    delete[] trait;

    for (int i=0; i<this->get_width(); i++)
    {
        delete[] tiles[i];
    }
    delete[] tiles;
}
