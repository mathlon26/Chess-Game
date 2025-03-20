#ifndef BISHOP__H
#define BISHOP__H

#include "Piece.h"
#include "Move.h"
#include "Player.h"

class Bishop : public Piece
{
public:
    Bishop(PieceColor color);
    ~Bishop() {};

    std::string Symbol() const override;
    std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const override;
    bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const override ;

private:
};

#endif // BISHOP__H