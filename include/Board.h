#ifndef BOARD__H
#define BOARD__H

#include "Piece.h"
#include "Move.h"
#include <array>

class Player;
class Piece;

// A structure that holds all information needed to undo a move.
struct MoveRecord {
    int x0, y0, x1, y1;
    Piece* movedPiece;        // The piece originally at the start square.
    Piece* capturedPiece;     // The piece that was captured (if any). For en passant, this is stored separately.
    Piece* replacementPiece;  // If a promotion occurred, this is the new piece.
    bool enPassantUsed;       // True if the move used an en passant capture.
    // Save the en passant state from before the move.
    bool prevEnPassantAvailable;
    int prevEnPassantX;
    int prevEnPassantY;
};



class Board
{
public:
    Board();
    ~Board();

    void Draw() const;
    bool ExecuteMove(Move &move, Player* executor);
    MoveRecord CommitMoveRecord(Move &move, Piece* replacement);
    void RevertMoveRecord(const MoveRecord &record);

    Piece* GetPiece(int x, int y) const;
    std::vector<Move> GetPossibleMoves(Color color) const;

    // En passant support.
    bool IsEnPassantTarget(int x, int y) const;
    void ClearEnPassantTarget();

    // Check support.
    bool IsKingChecked(Color color) const;
    bool SetKingChecked(Color color);
private:
    std::array<std::array<Piece*, 8>, 8> m_board;
    
    // En passant state.
    bool m_enPassantAvailable;
    int m_enPassantX;
    int m_enPassantY;

    // King state
    bool m_whiteKingInCheck;
    bool m_blackKingInCheck;
    
    // Helper function to initialize the board with all pieces.
    void initializeBoard();
};

#endif // BOARD__H
