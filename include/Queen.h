#ifndef QUEEN__H
#define QUEEN__H

#include "Piece.h"
#include "Move.h"
#include "Player.h"

class Queen : public Piece
{
public:
    Queen(PieceColor color);
    ~Queen() {};

    std::string Symbol() const override;
    std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const override;
    bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const override ;

private:
};

#endif // QUEEN__H