#include "../include/Queen.h"

Queen::Queen(PieceColor color)
    :
    Piece{color}
{
}


std::string Queen::Symbol() const 
{
    return "Q";
}

// Generates all possible moves for a queen from its current position.
// Combines the moves of the rook and bishop.
std::vector<Move> Queen::PossibleMoves(Board* board, int currentX, int currentY) const {
    std::vector<Move> moves;
    // Directions: rook-like (4) and bishop-like (4).
    int directions[8][2] = {
        {1,0}, {-1,0}, {0,1}, {0,-1},
        {1,1}, {1,-1}, {-1,1}, {-1,-1}
    };

    for (auto &dir : directions) {
        int x = currentX, y = currentY;
        while (true) {
            x += dir[0];
            y += dir[1];
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;
            Piece* piece = board->GetPiece(x, y);
            if (piece == nullptr) {
                moves.push_back(Move{currentX, currentY, x, y});
            } else {
                if (piece->Color() != m_color)
                    moves.push_back(Move{currentX, currentY, x, y});
                break;
            }
        }
    }
    return moves;
}

// Uses the list of possible moves to determine if the move is valid.
bool Queen::IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const {
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