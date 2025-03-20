#ifndef BOT_PLAYER__H
#define BOT_PLAYER__H

#include "Player.h"
#include <random>
#include <string>
#include <unordered_set>

class BotPlayer : public Player
{
public:
    BotPlayer(PlayerColor color);
    ~BotPlayer();

    bool IsBot() override;

    std::string GetInput(const Board* const board) const override;
    std::string Name() const override;
    void AddCache(std::string moveStr);
    void EmptyCache();
private:
    std::unordered_set<std::string> m_cache{};
};

#endif // BOT_PLAYER__H