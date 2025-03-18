#include "../include/Player.h"

Player::Player(Color color)
    :
    m_color{color}
{
}

Player::~Player()
{
}

Color Player::GetColor() 
{
    return m_color;
}