#include "../include/ChessGame.h"
#include "../include/HumanPlayer.h"
#include "../include/BotPlayer.h"
#include <sstream>
#include <thread>
#include <chrono>

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
        SwitchTurn();
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
        m_playerOne = new BotPlayer{PlayerColor::WHITE};
        m_playerTwo = new BotPlayer{PlayerColor::BLACK};
    }
    else
    {
        m_playerOne = new HumanPlayer{Color::WHITE};
        m_playerTwo = new HumanPlayer{Color::BLACK};
    }
    m_playerOnTurn = m_playerOne;

    ClearConsole();
}

void ChessGame::SetupTimers()
{
    bool optT{false}; // timer bool
    int optD{0}; // timer duration (minutes)

    ClearConsole();
    optT = GetBoolInput(" >> ", "Do you want to play with a timer?", Color::GREEN);

    do
    {
        optD = std::stoi(GetStringInput(" >> ", "Set the duration of the timer in minutes (>=1).").c_str());
    } while (optD < 1);

    if (optT)
    {
        // Setup timers with duration optD
    }
    
    
    ClearConsole();
}

void ChessGame::Init()
{
    SetupPlayers();
    // SetupTimers();
}

void ChessGame::EndGame(Player* winner, bool resign, bool notime)
{
    PrintBox(
        winner->Name() + " has won!",
        "Game ended."
    );

    Exit();
}

Move ChessGame::GetPlayerMove()
{
    Move playerMove{};
    while (true)
    {
        try
        {
            bool checked = m_board->IsKingChecked(m_playerOnTurn->Color());
            bool noMoves = m_board->GetPossibleMoves(m_playerOnTurn->Color()).empty();
            if (checked && noMoves) {
                EndGame(m_playerOnTurn == m_playerOne ? m_playerTwo : m_playerOne);
                return Move{};
            }
                

            std::string moveStr = m_playerOnTurn->GetInput(m_board);
            
            if (moveStr == "resign") {
                EndGame(m_playerOnTurn == m_playerOne ? m_playerTwo : m_playerOne, true);
                return Move{};
            }
            playerMove = Move{moveStr};
            return playerMove;
        }
        catch(const std::exception& e)
        {
            // We must make sure a BotPlayer returns valid input, otherwise this will be executed.
            PrintError("Invalid input, example format: 'e2e4' \n  | " + std::string{e.what()});
            continue;
        }
    }
}

void ChessGame::Update()
{
    bool succeed {false};
    Move playerMove;
    do
    {
        playerMove = GetPlayerMove();
        if (playerMove.empty())
            return;

        if (m_playerOnTurn->IsBot())
        {
            BotPlayer* bot = dynamic_cast<BotPlayer*>(m_playerOnTurn);
            bot->AddCache(playerMove.toString());
        }
        

        succeed = m_board->ExecuteMove(playerMove, m_playerOnTurn);
        if (!succeed && !m_playerOnTurn->IsBot())
            PrintError("Invalid Move!");
    } while (! succeed);

    if (m_playerOnTurn->IsBot())
    {
        PrintBox(Cta(m_playerOnTurn->Color()) + playerMove.toString(), "Bot");
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        BotPlayer* bot = dynamic_cast<BotPlayer*>(m_playerOnTurn);
        bot->EmptyCache();
    }
}

void ChessGame::Draw() const
{
    if (m_exitCall)
        return;
    
    ClearConsole();

    std::ostringstream oss;

    std::cout << "\033[0;0H\033[0m";

    oss << "Player " << m_playerOnTurn->Name() << " may make a move.\n"
        << m_playerOne->Name() << " has captured: ";

    for (auto capture : m_playerOne->GetCaptures())
        oss << capture->Symbol() << ' ';
    oss << '\n' << m_playerTwo->Name() << " has captured: ";

    for (auto capture : m_playerTwo->GetCaptures())
        oss << capture->Symbol() << ' ';

    PrintBox(oss.str(), " Game Stats ");

    m_board->Draw();

}