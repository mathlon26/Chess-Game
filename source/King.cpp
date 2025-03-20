#include "../include/King.h"

King::King(PieceColor color)
    :
    Piece{color}
{
}


std::string King::Symbol() const 
{
    return "K";
}

// Generates all possible moves for a king from its current position.
// Castling is not implemented here.
std::vector<Move> King::PossibleMoves(Board* board, int currentX, int currentY) const {
    std::vector<Move> moves;
    // All adjacent squares.
    int offsets[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
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
    // Note: Castling moves can be added here if needed.
    return moves;
}

// Uses the list of possible moves to determine if the move is valid.
bool King::IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const {
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