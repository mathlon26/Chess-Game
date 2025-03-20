#include "../include/Bishop.h"

Bishop::Bishop(PieceColor color)
    :
    Piece{color}
{
}


std::string Bishop::Symbol() const 
{
    return "B";
}

// Generates all possible moves for a bishop from its current position.
std::vector<Move> Bishop::PossibleMoves(Board* board, int currentX, int currentY) const {
    std::vector<Move> moves;
    // Diagonal directions.
    int directions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

    for (auto &dir : directions) {
        int x = currentX, y = currentY;
        while (true) {
            x += dir[0];
            y += dir[1];
            // Check bounds.
            if (x < 0 || x >= 8 || y < 0 || y >= 8)
                break;
            Piece* piece = board->GetPiece(x, y);
            if (piece == nullptr) {
                moves.push_back(Move{currentX, currentY, x, y});
            } else {
                // Capture enemy piece.
                if (piece->Color() != m_color)
                    moves.push_back(Move{currentX, currentY, x, y});
                break;
            }
        }
    }
    return moves;
}

// Uses the list of possible moves to determine if the move is valid.
bool Bishop::IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const {
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