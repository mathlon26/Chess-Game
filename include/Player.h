#ifndef PLAYER__H
#define PLAYER__H

#include "Move.h"
#include "Piece.h"
#include "../dependencies/Style.hpp"
#include <string>

typedef Color PlayerColor;

class Player
{
public:
    Player(PlayerColor color);
    virtual ~Player() = 0;

    virtual std::string GetInput(const Board* const board) const = 0;
    virtual std::string Name() const = 0;

    virtual bool IsBot() {return false;}

    PlayerColor Color() const;
    std::vector<Piece *> GetCaptures() const;
    void AddCapture(Piece *captured);

protected:
    PlayerColor m_color;
    std::vector<Piece *> m_captures;
};

#endif // PLAYER__H