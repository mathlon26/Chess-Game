#ifndef KNIGHT__H
#define KNIGHT__H

#include "Piece.h"
#include "Move.h"
#include "Player.h"

class Knight : public Piece
{
public:
    Knight(PieceColor color);
    ~Knight() {};

    std::string Symbol() const override;
    std::vector<Move> PossibleMoves(Board* board, int currentX, int currentY) const override;
    bool IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const override ;

private:
};

#endif // KNIGHT__H