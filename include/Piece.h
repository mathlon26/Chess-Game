#ifndef PIECE__H
#define PIECE__H

#include "../dependencies/Style.hpp"

class Piece
{
public:
    Piece(Color color) : m_color{color} {}
    virtual ~Piece() = default;

    virtual std::string GetSymbol() = 0;

    Color GetColor() {return m_color;}
protected:
    Color m_color;
};

#endif // PIECE__H