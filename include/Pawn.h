#ifndef PAWN__H
#define PAWN__H

#include "Piece.h"
#include "Move.h"

class Pawn : public Piece
{
public:
    Pawn(PieceColor color);
    ~Pawn() {};

    std::string Symbol() const override;
    std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const override;
    bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const override ;

private:
};

#endif // PAWN__H