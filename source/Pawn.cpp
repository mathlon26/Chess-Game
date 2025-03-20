#include "../include/Pawn.h"
#include "../include/Rook.h"
#include "../include/Bishop.h"
#include "../include/Knight.h"
#include "../include/Queen.h"
#include "../include/Board.h"
#include "../include/Move.h"
#include <cmath>

// Constructor.
Pawn::Pawn(PieceColor color) : Piece{color} 
{
}

// Helper function to promote a pawn.
void promote(Piece* &setReplacement, PieceColor color)
{
    std::string promotion = Style::GetStringInput(
        "Choose from:\n Queen\nRook\nBishop\nKnight\n >> ",
        Style::Cta(color) + Style::Cts(color) + Style::Cta(Color::GREEN) + " may promote their pawn."
    );
    if (promotion == "Rook") {
        setReplacement = new Rook{color};
    } else if (promotion == "Bishop") {
        setReplacement = new Bishop{color};
    } else if (promotion == "Knight") {
        setReplacement = new Knight{color};
    } else {
        setReplacement = new Queen{color};
    }
}

// Generates all possible moves for a pawn from its current position.
std::vector<Move> Pawn::PossibleMoves(Board* board, int currentX, int currentY) const 
{
    std::vector<Move> moves;
    int direction    = (m_color == PieceColor::WHITE) ? -1 : 1;
    int startRow     = (m_color == PieceColor::WHITE) ? 6 : 1;
    int promotionRow = (m_color == PieceColor::WHITE) ? 0 : 7;
    
    int forwardY = currentY + direction;
    // Make sure the forward square is within bounds.
    if (forwardY >= 0 && forwardY < 8) {
        // Single-step forward move.
        if (board->GetPiece(currentX, forwardY) == nullptr) {
            Move move{currentX, currentY, currentX, forwardY};
            moves.push_back(move);
            
            // Two-step forward move (only from the starting row).
            if (currentY == startRow) {
                int twoStepY = currentY + 2 * direction;
                if (twoStepY >= 0 && twoStepY < 8 && board->GetPiece(currentX, twoStepY) == nullptr)
                {
                move = Move{currentX, currentY, currentX, twoStepY};
                moves.push_back(move);
                }
            }
        }
    }
    
    // Diagonal moves: captures and en passant.
    for (int dx = -1; dx <= 1; dx += 2) {
        int newX = currentX + dx;
        int newY = currentY + direction;
        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            Piece* diagPiece = board->GetPiece(newX, newY);
            // Standard capture: enemy piece present.
            if (diagPiece != nullptr && diagPiece->Color() != m_color) {
                Move move{currentX, currentY, newX, newY};
                moves.push_back(move);
            }
            // En passant capture: target square is empty but is flagged.
            else if (diagPiece == nullptr && board->IsEnPassantTarget(newX, newY)) {
                Move move{currentX, currentY, newX, newY};
                moves.push_back(move);
            }
        }
    }

    return moves;
}

// Uses the list of possible moves to determine if the move is valid.
// Also handles promotion if the move ends on the promotion row.
bool Pawn::IsValidMove(Piece* target, Board* board, Move& move, Piece*& setReplacement) const
{
    int startX = move.getStartX();
    int startY = move.getStartY();
    int endX   = move.getEndX();
    int endY   = move.getEndY();
    
    // Retrieve candidate moves for this pawn from its current position.
    std::vector<Move> candidates = PossibleMoves(board, startX, startY);

    bool found = false;
    for (const Move& candidate : candidates) {
        if (candidate.getStartX() == move.getStartX() &&
            candidate.getStartY() == move.getStartY() &&
            candidate.getEndX()   == move.getEndX() &&
            candidate.getEndY()   == move.getEndY())
        {
            found = true;
            break;
        }
    }
    
    if (!found)
        return false;
    
    // Check for promotion: if the move ends on the promotion row, prompt for promotion.
    int promotionRow = (m_color == PieceColor::WHITE) ? 7 : 0;
    if (endY == promotionRow)
        promote(setReplacement, m_color);
    
    return true;
}

std::string Pawn::Symbol() const 
{
    return "P";
}
