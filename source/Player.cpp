#include "../include/Player.h"

Player::Player(PlayerColor color)
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

void Player::AddCapture(Piece* captured)
{
    m_captures.push_back(captured);
}

PlayerColor Player::Color() const
{
    return m_color;
}