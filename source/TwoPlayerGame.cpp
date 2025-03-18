#include "../include/TwoPlayerGame.h"

TwoPlayerGame::TwoPlayerGame() 
{
}

TwoPlayerGame::~TwoPlayerGame() 
{
    delete m_playerOne;
    delete m_playerTwo;
}

Player* TwoPlayerGame::GetPlayerOnTurn()
{
    return m_playerOnTurn;
}

Player* TwoPlayerGame::SwitchTurn()
{
    m_playerOnTurn = (m_playerOnTurn == m_playerOne) ? m_playerTwo : m_playerOne;
    return m_playerOnTurn;
}