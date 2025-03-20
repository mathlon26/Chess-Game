#ifndef KING__H
#define KING__H

#include "Piece.h"
#include "Move.h"

class King : public Piece
{
public:
    King(PieceColor color);
    ~King() {};

    std::string Symbol() const override;
    std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const override;
    bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const override ;
    bool IsKing() const override {return true;}
private:
};

#endif // KING__H