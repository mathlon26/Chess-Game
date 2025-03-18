#include "../include/HumanPlayer.h"
#include "../dependencies/Style.hpp"

HumanPlayer::HumanPlayer(Color color)
    :
    Player{color}
{
}

HumanPlayer::~HumanPlayer()
{
}

std::string HumanPlayer::GetName() const
{
    return Style::Cts(m_color) + " (Player)";
}

std::string HumanPlayer::GetInput() const
{
    return Style::GetStringInput(
        " >> ",
        Style::Cta(m_color) + Style::Cts(m_color) + Style::Cta(Color::GREEN) + " may make a move."
    );
}