#ifndef PIECE__H
#define PIECE__H

#include "../dependencies/Style.hpp"
#include "Board.h"
#include "Move.h"

// for Color() function
typedef Color PieceColor;

class Move;
class Board;

class Piece
{
public:
    Piece(PieceColor color) : m_color{color} {}
    virtual ~Piece() = default;
    PieceColor Color() const {return m_color;}

    

    virtual std::string Symbol() const = 0;
    virtual std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const = 0;
    virtual bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const = 0;

    virtual bool IsKing() const {return false;}

protected:
    
    PieceColor m_color;
};

#endif // PIECE__H