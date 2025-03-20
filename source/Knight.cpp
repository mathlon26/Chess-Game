#include "../include/Knight.h"

Knight::Knight(PieceColor color)
    :
    Piece{color}
{
}


std::string Knight::Symbol() const 
{
    return "H";
}

// Generates all possible moves for a knight from its current position.
std::vector<Move> Knight::PossibleMoves(Board* board, int currentX, int currentY) const {
    std::vector<Move> moves;
    // All L-shaped moves.
    int offsets[8][2] = {
        {1, 2}, {2, 1}, {2, -1}, {1, -2},
        {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    };

    for (auto &off : offsets) {
        int x = currentX + off[0];
        int y = currentY + off[1];
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            Piece* piece = board->GetPiece(x, y);
            if (piece == nullptr || piece->Color() != m_color)
                moves.push_back(Move{currentX, currentY, x, y});
        }
    }
    return moves;
}

// Uses the list of possible moves to determine if the move is valid.
bool Knight::IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const {
    auto candidates = PossibleMoves(board, move.getStartX(), move.getStartY());
    for (const Move& candidate : candidates) {
        if (candidate.getStartX() == move.getStartX() &&
            candidate.getStartY() == move.getStartY() &&
            candidate.getEndX() == move.getEndX() &&
            candidate.getEndY() == move.getEndY())
            return true;
    }
    return false;
}