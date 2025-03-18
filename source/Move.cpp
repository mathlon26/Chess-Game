#include "../include/Move.h"
#include <stdexcept>

// Default controller
Move::Move()
    : Move{0,0,0,0}
{
}

// Constructor with numeric coordinates
Move::Move(int startX, int startY, int endX, int endY)
    : startX(startX), startY(startY), endX(endX), endY(endY)
{
    validateCoordinates(startX, startY);
    validateCoordinates(endX, endY);
}

// Constructor that accepts a move string, e.g., "e2e4"
Move::Move(const std::string& moveStr)
{
    parseMoveString(moveStr);
}

// Helper function to check if a given coordinate pair is within the chessboard bounds (0-7)
void Move::validateCoordinates(int x, int y) const
{
    if (x < 0 || x >= 8 || y < 0 || y >= 8)
    {
        throw std::out_of_range("Coordinates out of bounds. Valid range is 0 to 7 for both x and y.");
    }
}

// Parses a move string of the format \"[a-h][1-8][a-h][1-8]\" and sets the coordinates accordingly
void Move::parseMoveString(const std::string& moveStr)
{
    if (moveStr.length() != 4)
    {
        throw std::invalid_argument("Move string must be exactly 4 characters long.");
    }

    char startFile = moveStr[0];
    char startRank = moveStr[1];
    char endFile = moveStr[2];
    char endRank = moveStr[3];

    if (startFile < 'a' || startFile > 'h' ||
        endFile   < 'a' || endFile   > 'h' ||
        startRank < '1' || startRank > '8' ||
        endRank   < '1' || endRank   > '8')
    {
        throw std::invalid_argument("Invalid move string format. Must be in format [a-h][1-8][a-h][1-8].");
    }

    // Convert the algebraic notation into 0-indexed board coordinates
    startX = startFile - 'a';
    startY = startRank - '1';
    endX   = endFile   - 'a';
    endY   = endRank   - '1';

    // Validate the computed coordinates
    validateCoordinates(startX, startY);
    validateCoordinates(endX, endY);
}

// Checks whether the stored move coordinates are valid within an 8x8 chessboard
bool Move::isValid() const
{
    return (startX >= 0 && startX < 8 &&
            startY >= 0 && startY < 8 &&
            endX   >= 0 && endX   < 8 &&
            endY   >= 0 && endY   < 8);
}

// Converts the move back into algebraic chess notation (e.g., \"e2e4\")
std::string Move::toString() const
{
    char startFile = 'a' + startX;
    char startRank = '1' + startY;
    char endFile   = 'a' + endX;
    char endRank   = '1' + endY;
    return std::string() + startFile + startRank + endFile + endRank;
}

// Comparison operator to check if two moves are identical
bool Move::operator==(const Move& other) const
{
    return (startX == other.startX &&
            startY == other.startY &&
            endX   == other.endX &&
            endY   == other.endY);
}
