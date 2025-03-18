#ifndef BOARD__H
#define BOARD__H

#include "Piece.h"
#include <array>

class Board
{
public:
    Board();
    ~Board();

    void Draw() const;
    
private:
    std::array<std::array<Piece*, 8>, 8> m_board;
};

#endif // BOARD__H