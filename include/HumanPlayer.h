#ifndef HUMANE_PLAYER__H
#define HUMANE_PLAYER__H

#include "Player.h"

class HumanPlayer : public Player
{
public:
    HumanPlayer(Color color);
    ~HumanPlayer();

    std::string GetInput() const override;
    std::string GetName() const override;
private:
};

#endif // HUMANE_PLAYER__H