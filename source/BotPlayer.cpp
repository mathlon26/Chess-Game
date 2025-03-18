#include "../include/BotPlayer.h"

BotPlayer::BotPlayer(Color color)
    :
    Player{color}
{

}

BotPlayer::~BotPlayer()
{
}

std::string BotPlayer::GetName() const
{
    return Style::Cts(m_color) + " (Bot)";
}

std::string BotPlayer::GetInput() const
{
    return std::string{};
}