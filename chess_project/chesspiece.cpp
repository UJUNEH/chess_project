#include "chesspiece.h"

using namespace std;


ChessPiece::ChessPiece(string color, string type, int x, int y, int flag)
{
    this->color = color;
    this->type = type;
    this->x = x;
    this->y = y;
    this->flag = flag;
}

ostream& operator<<(ostream& os, const ChessPiece& piece)
{
    char c;
    c = piece_to_char[piece.type];
    if (piece.color == "Black")
    {
        c = tolower(c);
    }
    os << c;
    return os;
}

ChessPiece::ChessPiece(const ChessPiece& other)
{
    this->color = other.color;
    this->type = other.type;
    this->x = other.x;
    this->y = other.y;
    this->flag = other.flag;
}

ChessPiece* create_piece(string color, string type, int x, int y, int flag)
{
    if (type == "King") return new King(color, x, y, flag);
    else if (type == "Queen") return new Queen(color, x, y, flag);
    else if (type == "Rook") return new Rook(color, x, y, flag); 
    else if (type == "Bishop") return new Bishop(color, x, y, flag);
    else if (type == "Knight") return new Knight(color, x, y, flag);  
    else if (type == "Pawn") return new Pawn(color, x, y, flag);
    else return nullptr;
}
