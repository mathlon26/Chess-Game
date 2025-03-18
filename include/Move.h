#ifndef MOVE__H
#define MOVE__H

#include <string>
#include <iostream>
#include <stdexcept>

class Move
{
public:
    // Constructors
    Move();
    Move(int startX, int startY, int endX, int endY);
    Move(const std::string& moveStr); // e.g., "e2e4"

    // Destructor
    ~Move() = default;

    // Accessors
    int getStartX() const { return startX; }
    int getStartY() const { return startY; }
    int getEndX() const { return endX; }
    int getEndY() const { return endY; }

    // Utility Functions
    bool isValid() const;  // Validates if the move is within chessboard bounds
    std::string toString() const; // Converts back to chess notation

    // Comparison Operator
    bool operator==(const Move& other) const;

private:
    int startX, startY;
    int endX, endY;

    // Helper functions
    void validateCoordinates(int x, int y) const;
    void parseMoveString(const std::string& moveStr);
};

#endif // MOVE_H