#include "../include/ChessGame.h"
#include "../include/HumanPlayer.h"
#include "../include/BotPlayer.h"
#include <sstream>

ChessGame::ChessGame()
    :
    m_board{new Board{}}
{
}

ChessGame::~ChessGame()
{
}

void ChessGame::Run()
{
    Init();

    Draw();
    while (ShouldRun())
    {
        Update();
        Draw();
    }
}

void ChessGame::SetupPlayers()
{
    int optP; // player versus X option
    int optC{1}; // player color option
    do
    {
        ClearConsole();
        PrintBox(
            "1. Player versus Player\n"
            "2. Player versus Bot",
            " Game Mode "
        );
        optP = std::stoi(GetStringInput(" >> ", "Enter a number (1,2)").c_str());
    } while (optP != 1 && optP != 2 && optP != 3); // optP 3 is hidden to play Bot V Bot

    if (optP == 2)
    {
        do
        {
            ClearConsole();
            PrintBox(
                "1. Player plays white\n"
                "2. Bot plays white",
                " Player Color "
            );
            optC = std::stoi(GetStringInput(" >> ", "Enter a number (1,2)").c_str());
        } while (optC != 1 && optC != 2);

        if (optC == 1)
        {
            m_playerOne = new HumanPlayer{Color::WHITE};
            m_playerTwo = new BotPlayer{Color::BLACK};
        }
        else
        {
            m_playerOne = new BotPlayer{Color::WHITE};
            m_playerTwo = new HumanPlayer{Color::BLACK};
        }
    }
    else if (optP == 3)
    {
        m_playerOne = new BotPlayer{Color::WHITE};
        m_playerTwo = new BotPlayer{Color::BLACK};
    }
    else
    {
        m_playerOne = new HumanPlayer{Color::WHITE};
        m_playerTwo = new HumanPlayer{Color::BLACK};
    }
    m_playerOnTurn = m_playerTwo; // black player because on update the turn is toggled

    ClearConsole();
}

void ChessGame::Init()
{
    SetupPlayers();
}

Move ChessGame::GetPlayerMove()
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
    SwitchTurn();
    Move playerMove = GetPlayerMove();
}

void ChessGame::Draw() const
{
    ClearConsole();

    std::ostringstream oss;

    std::cout << "\033[0;0H\033[0m";

    oss << "Player " << m_playerOnTurn->GetName() << " may make a move.\n"
        << m_playerOne->GetName() << " has captured: ";

    for (auto capture : m_playerOne->GetCaptures())
        oss << capture->GetSymbol() << ' ';
    oss << '\n' << m_playerTwo->GetName() << " has captured: ";

    for (auto capture : m_playerTwo->GetCaptures())
        oss << capture->GetSymbol() << ' ';

    PrintBox(oss.str(), " Game Stats ");

    m_board->Draw();

}