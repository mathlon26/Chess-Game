#ifndef HUMANE_PLAYER__H
#define HUMANE_PLAYER__H

#include "Player.h"

class HumanPlayer : public Player
{
public:
    HumanPlayer(PlayerColor color);
    ~HumanPlayer();

    std::string GetInput(const Board* const board) const override;
    std::string Name() const override;
private:
};

#endif // HUMANE_PLAYER__H