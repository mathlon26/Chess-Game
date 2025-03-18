#include "../include/Player.h"

Player::Player(Color color)
    :
    m_color{color},
    m_captures{}
{
}

Player::~Player()
{
}

std::vector<Piece*> Player::GetCaptures() const
{
    return m_captures;
}

Color Player::GetColor() const
{
    return m_color;
}