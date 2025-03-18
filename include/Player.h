#ifndef PLAYER__H
#define PLAYER__H

#include "Move.h"
#include "Piece.h"
#include "../dependencies/Style.hpp"
#include <string>

class Player
{
public:
    Player(Color color);
    virtual ~Player() = 0;

    virtual std::string GetInput() const = 0;
    virtual std::string GetName() const = 0;

    Color GetColor() const;
    std::vector<Piece*> GetCaptures() const;
protected:
    Color m_color;
    std::vector<Piece*> m_captures;
};

#endif // PLAYER__H