#ifndef PLAYER__H
#define PLAYER__H

#include "../include/Move.h"
#include "../dependencies/Style.hpp"
#include <string>

class Player
{
public:
    Player(Color color);
    ~Player();

    Color GetColor() {return m_color;}

    virtual std::string GetInput() = 0;
private:
    Color m_color;
};

#endif // PLAYER__H