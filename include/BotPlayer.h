#ifndef BOT_PLAYER__H
#define BOT_PLAYER__H

#include "Player.h"

class BotPlayer : public Player
{
public:
    BotPlayer(Color color);
    ~BotPlayer();

    std::string GetInput() const override;
    std::string GetName() const override;
private:
};

#endif // BOT_PLAYER__H