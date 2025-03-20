#ifndef ROOK__H
#define ROOK__H

#include "Piece.h"
#include "Move.h"
#include "Player.h"

class Rook : public Piece
{
public:
    Rook(PieceColor color);
    ~Rook() {};

    std::string Symbol() const override;
    std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const override;
    bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const override ;

private:
};

#endif // ROOK__H