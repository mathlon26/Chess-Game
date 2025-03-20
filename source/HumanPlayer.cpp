#include "../include/HumanPlayer.h"
#include "../dependencies/Style.hpp"

HumanPlayer::HumanPlayer(PlayerColor color)
    :
    Player{color}
{
}

HumanPlayer::~HumanPlayer()
{
    for (Piece* piece : m_captures)
        delete piece;
}

std::string HumanPlayer::Name() const
{
    return Style::Cts(m_color) + " (Player)";
}

std::string HumanPlayer::GetInput(const Board* const board) const
{
    return Style::GetStringInput(
        " >> ",
        Style::Cta(m_color) + Style::Cts(m_color) + Style::Cta(Color::GREEN) + " may make a move."
    );
}