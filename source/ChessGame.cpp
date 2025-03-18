#include "../include/ChessGame.h"
#include "ChessGame.h"

ChessGame::ChessGame()
{
}

ChessGame::~ChessGame()
{
}

void ChessGame::Run()
{
    Init();
    SetupPlayers();

    Draw();
    while (ShouldRun())
    {
        Update();
        Draw();
    }
}

void ChessGame::SetupPlayers()
{

}

void ChessGame::Init()
{

}

Move &ChessGame::GetPlayerMove()
{
    Move playerMove{};
    while (true)
    {
        try
        {
            playerMove = Move{m_playerOnTurn->GetInput()};
            return playerMove;
        }
        catch(const std::exception& e)
        {
            // We must make sure a BotPlayer returns valid input, otherwise this will be executed.
            PrintError("Invalid input, example format: 'e2e4' " + std::string{e.what()});
            continue;
        }
    }
}

void ChessGame::Update()
{
    Move &playerMove = GetPlayerMove();
}

void ChessGame::Draw() const
{
    
}