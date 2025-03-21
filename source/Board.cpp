#include "../include/Board.h"
#include "../include/Piece.h"
#include "../include/Player.h"
#include "../include/Pawn.h"
#include "../include/Rook.h"
#include "../include/Knight.h"
#include "../include/Bishop.h"
#include "../include/Queen.h"
#include "../include/King.h"  // Assumes you have a King class implemented.
#include "../include/Move.h"
#include "../dependencies/Style.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>



// Constructor.
Board::Board()
    : m_enPassantAvailable(false), m_enPassantX(-1), m_enPassantY(-1)
{
    // Initialize board squares to nullptr.
    for (auto& row : m_board)
        row.fill(nullptr);
    
    // Set up pieces in their starting positions.
    initializeBoard();
}

// Destructor.
Board::~Board()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            delete m_board[i][j];
}

void Board::initializeBoard()
{
    // --- White Pieces ---
    // Major pieces on row 0.
    m_board[0][0] = new Rook(PieceColor::BLACK);
    m_board[0][1] = new Knight(PieceColor::BLACK);
    m_board[0][2] = new Bishop(PieceColor::BLACK);
    m_board[0][3] = new Queen(PieceColor::BLACK);
    m_board[0][4] = new King(PieceColor::BLACK);
    m_board[0][5] = new Bishop(PieceColor::BLACK);
    m_board[0][6] = new Knight(PieceColor::BLACK);
    m_board[0][7] = new Rook(PieceColor::BLACK);
    // Pawns on row 1.
    for (int x = 0; x < 8; ++x)
        m_board[1][x] = new Pawn(PieceColor::BLACK);
    
    // --- Black Pieces ---
    // Pawns on row 6.
    for (int x = 0; x < 8; ++x)
        m_board[6][x] = new Pawn(PieceColor::WHITE);
    // Major pieces on row 7.
    m_board[7][0] = new Rook(PieceColor::WHITE);
    m_board[7][1] = new Knight(PieceColor::WHITE);
    m_board[7][2] = new Bishop(PieceColor::WHITE);
    m_board[7][3] = new Queen(PieceColor::WHITE);
    m_board[7][4] = new King(PieceColor::WHITE);
    m_board[7][5] = new Bishop(PieceColor::WHITE);
    m_board[7][6] = new Knight(PieceColor::WHITE);
    m_board[7][7] = new Rook(PieceColor::WHITE);
}

void Board::Draw() const
{
    std::ostringstream oss;
    oss << "   a  b  c  d  e  f  g  h\n";

    int count{0};

    for (int i = 0; i < 8; ++i)  {
        oss << 8 - i << " ";

        for (int j = 0; j < 8; ++j) {
            if (count++ % 2 == 0)
                oss << "\033[44m";
            else
                oss << "\033[42m";

            if (m_board[i][j] == nullptr)
                oss << "   ";
            else
            {
                oss << Style::Cta(m_board[i][j]->Color());
                oss << ' ' << m_board[i][j]->Symbol() << ' ';
            }
            oss << "\033[0m";
        }
        oss << ' ' << 8 - i << '\n';
        count++;
    }
    oss << "\033[0m   a  b  c  d  e  f  g  h" << std::endl;
    Style::PrintBox(oss.str(), " Board ");
}

void LogMoveRecord(const MoveRecord& record) {
    std::cout << "Move Record:\n";
    std::cout << "  From: " << char('a' + record.x0) << 8 - record.y0 
              << "  To: " << char('a' + record.x1) << 8 - record.y1 << "\n";

    std::cout << "  Moved Piece: " 
              << (record.movedPiece ? record.movedPiece->Symbol() : "None") << "\n";

    std::cout << "  Captured Piece: " 
              << (record.capturedPiece ? record.capturedPiece->Symbol() : "None") << "\n";

    std::cout << "  Replacement Piece (Promotion): " 
              << (record.replacementPiece ? record.replacementPiece->Symbol() : "None") << "\n";

    std::cout << "  En Passant Used: " 
              << (record.enPassantUsed ? "Yes" : "No") << "\n";

    std::cout << "  Previous En Passant State:\n";
    std::cout << "    Available: " 
              << (record.prevEnPassantAvailable ? "Yes" : "No") << "\n";

    if (record.prevEnPassantAvailable) {
        std::cout << "    Position: " 
                  << char('a' + record.prevEnPassantX) 
                  << 8 - record.prevEnPassantY << "\n";
    }

    std::cout << "-----------------------------------------\n";
}


bool Board::ExecuteMove(Move &move, Player* executor)
{
    int x0 = move.getStartX(), y0 = move.getStartY();
    int x1 = move.getEndX(), y1 = move.getEndY();

    Piece* moving = m_board[y0][x0];
    Piece* target = m_board[y1][x1];  // Target piece (if any)
    Piece* replacement = nullptr;

    if (!moving || moving->Color() != executor->Color()) {
        return false;
    }

    // Ensure the move is valid for the selected piece.
    if (!moving->IsValidMove(target, this, move, replacement)) {
        return false;
    }

    bool wasInCheck = IsKingChecked(executor->Color());

    // Execute the move and capture the move state.
    MoveRecord record = CommitMoveRecord(move, replacement);

    // If the king was in check and the move doesn't resolve it, revert.
    if (wasInCheck && IsKingChecked(executor->Color())) {
        RevertMoveRecord(record);
        
        return false;
    }

    // Check if the move puts the player's own king in check.
    if (IsKingChecked(executor->Color())) {
        // Revert the move using the saved state.
        RevertMoveRecord(record);
        return false;
    }

    // If a capture was made, record it.
    if (record.capturedPiece)
        executor->AddCapture(record.capturedPiece);


    // Update check state for opponent.
    SetKingChecked(moving->Color() == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE);

    // Handle en passant state if applicable.
    if (dynamic_cast<Pawn*>(moving) && std::abs(y1 - y0) == 2) {
        m_enPassantAvailable = true;
        m_enPassantX = x0;
        m_enPassantY = y0 + (y1 - y0) / 2;
    } else {
        ClearEnPassantTarget();
    }

    return true;
}


Piece* Board::GetPiece(int x, int y) const 
{
    return m_board[y][x];
}

bool Board::IsEnPassantTarget(int x, int y) const
{
    return m_enPassantAvailable && (x == m_enPassantX) && (y == m_enPassantY);
}

void Board::ClearEnPassantTarget()
{
    m_enPassantAvailable = false;
    m_enPassantX = -1;
    m_enPassantY = -1;
}

// Returns true if the king of the specified color is in check.
bool Board::IsKingChecked(Color color) const
{

    // Find the king of the specified color.
    int kingX = -1, kingY = -1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = m_board[i][j];
            if (piece != nullptr && piece->Color() == color && piece->IsKing()) {
                kingX = j;
                kingY = i;
                break;
            }
        }
        if (kingX != -1)
            break;
    }
    
    // If king was not found (should not happen in a standard game), return false.
    if (kingX == -1)
        return false;

    // Check every enemy piece to see if it has a valid move to the king's position.
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = m_board[i][j];
            if (piece != nullptr && piece->Color() != color) {
                // Create a temporary move from the enemy piece's position to the king's position.
                Move testMove(j, i, kingX, kingY);
                Piece* replacement = nullptr;
                if (piece->IsValidMove(m_board[kingY][kingX], const_cast<Board*>(this), testMove, replacement))
                {
                    return true; // King is under attack.
                }
            }
        }
    }
    return false; // No enemy piece can legally move to the king's square.
}

// Updates the check state for the king of the given color and returns the result.
bool Board::SetKingChecked(Color color)
{
    bool inCheck = IsKingChecked(color);
    if (color == PieceColor::WHITE)
        m_whiteKingInCheck = inCheck;
    else
        m_blackKingInCheck = inCheck;
    return inCheck;
}

/// CommitMoveRecord:
/// Performs the move on the board and returns a MoveRecord capturing the previous state.
MoveRecord Board::CommitMoveRecord(Move &move, Piece* replacement)
{
    int x0 = move.getStartX();
    int y0 = move.getStartY();
    int x1 = move.getEndX();
    int y1 = move.getEndY();

    MoveRecord record;
    record.x0 = x0;
    record.y0 = y0;
    record.x1 = x1;
    record.y1 = y1;
    record.prevEnPassantAvailable = m_enPassantAvailable;
    record.prevEnPassantX = m_enPassantX;
    record.prevEnPassantY = m_enPassantY;
    
    record.movedPiece = m_board[y0][x0];
    record.replacementPiece = replacement;  // Will be non-null for a promotion.
    record.enPassantUsed = false;

    // By default, assume the captured piece is on the target square.
    record.capturedPiece = m_board[y1][x1];

    // Check for en passant capture: if a pawn moves diagonally into an empty square
    // that is flagged as an en passant target, then the captured pawn is adjacent.
    if (dynamic_cast<Pawn*>(record.movedPiece) &&
        std::abs(x1 - x0) == 1 && 
        record.capturedPiece == nullptr &&
        IsEnPassantTarget(x1, y1))
    {
         // For en passant, the captured pawn is at (x1, y0).
         record.capturedPiece = m_board[y0][x1];
         m_board[y0][x1] = nullptr;
         record.enPassantUsed = true;
         ClearEnPassantTarget();
    }

    // Perform the move.
    // If a promotion occurred (replacement non-null), place the replacement piece.
    m_board[y1][x1] = (replacement != nullptr) ? replacement : record.movedPiece;
    m_board[y0][x0] = nullptr;

    return record;
}

/// RevertMoveRecord:
/// Reverts a move using the information stored in the MoveRecord.
void Board::RevertMoveRecord(const MoveRecord &record)
{
    // If a promotion occurred, the piece on the destination is the replacement.
    // We delete it and restore the original pawn.
    if (record.replacementPiece != nullptr) {
        delete m_board[record.y1][record.x1];
        m_board[record.y1][record.x1] = record.movedPiece;
    } else {
        // For an en passant capture, the destination square should be empty,
        // and the captured pawn must be restored to its original square.
        if (record.enPassantUsed) {
            m_board[record.y0][record.x0] = record.movedPiece;
            m_board[record.y1][record.x1] = nullptr;
            m_board[record.y0][record.x1] = record.capturedPiece;
        } else {
            // Standard move: restore the moved piece and captured piece.
            m_board[record.y0][record.x0] = record.movedPiece;
            m_board[record.y1][record.x1] = record.capturedPiece;
        }
    }
    // Restore the en passant state.
    m_enPassantAvailable = record.prevEnPassantAvailable;
    m_enPassantX = record.prevEnPassantX;
    m_enPassantY = record.prevEnPassantY;
}


std::vector<Move> Board::GetPossibleMoves(Color color) const
{
    std::vector<Move> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece* piece = m_board[i][j];
            if (piece != nullptr && piece->Color() == color) {
                std::vector<Move> pieceMoves = piece->PossibleMoves(const_cast<Board*>(this), j, i);
                // Insert the candidate moves into our vector.
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }

    return moves;
}