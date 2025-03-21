#include "../include/ChessGame.h"
#include "../include/HumanPlayer.h"
#include "../include/BotPlayer.h"
#include <sstream>


ChessGame::ChessGame()
    :
    m_board{new Board{}},
    m_timerOne{0},
    m_timerTwo{0},
    m_increment{0}
{
}

ChessGame::~ChessGame()
{
}

void ChessGame::Run()
{
    bool first = true;
    Init();
    
    while (ShouldRun())
    {
        StartTimer();

        Update();
        Draw();

        StopTimer();
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
    int optI{0}; // timer increment (seconds)

    ClearConsole();
    optT = GetBoolInput(" >> ", "Do you want to play with a timer?", Color::GREEN);

    if (optT)
    {
        do
        {
            optD = std::stoi(GetStringInput(" >> ", "Set the duration of the timer in minutes (>=1).").c_str());
        } while (optD < 1);

        do
        {
            optI = std::stoi(GetStringInput(" >> ", "Set the increment of the timer in seconds (>=0).").c_str());
        } while (optI < 0);

        m_increment = optI;

        // Initialize both timers to the user-set duration in seconds.
        m_timerOne = optD * 60;
        m_timerTwo = optD * 60;

        // Enable timer thread.
        m_timerActive = true;
        m_timerThread = std::thread(&ChessGame::TimerThreadFunction, this);
    }
    
    ClearConsole();
}

void ChessGame::TimerThreadFunction() {
    while (m_timerActive && !m_exitCall)
    {
        // Get the current time and compute elapsed seconds since the move started.
        auto now = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - m_moveStartTime).count();

        // Compute remaining time for each player.
        // For the active player, subtract the elapsed time.
        long whiteRemaining = m_timerOne;
        long blackRemaining = m_timerTwo;
        if (m_playerOnTurn->Color() == Color::WHITE) {
            whiteRemaining = (m_timerOne > elapsedSeconds) ? m_timerOne - elapsedSeconds : 0;
            if (whiteRemaining == 0)
            {
                EndGame(m_playerTwo);
            }
        }
        else if (m_playerOnTurn->Color() == Color::BLACK) {
            blackRemaining = (m_timerTwo > elapsedSeconds) ? m_timerTwo - elapsedSeconds : 0;
            if (blackRemaining == 0)
            {
                EndGame(m_playerOne);
            }
            
        }

        // Convert seconds to minutes and seconds.
        int whiteMinutes = static_cast<int>(whiteRemaining / 60);
        int whiteSeconds = static_cast<int>(whiteRemaining % 60);
        int blackMinutes = static_cast<int>(blackRemaining / 60);
        int blackSeconds = static_cast<int>(blackRemaining % 60);

        // Save the current cursor position.
        std::cout << "\033[s";  // ANSI escape code to save cursor position

        // Print White's timer at row 1, column 54.
        std::cout << "\033[2;44H";  // Move cursor to row 1, column 54.
        std::cout << BoldText(Cta(Color::WHITE) + "White: ") << Cts(Color::DEFAULT)
                  << (whiteMinutes < 10 ? "0" : "") << whiteMinutes << ":"
                  << (whiteSeconds < 10 ? "0" : "") << whiteSeconds;

        // Print Black's timer at row 2, column 54.
        std::cout << "\033[3;44H";  // Move cursor to row 2, column 54.
        std::cout << BoldText(Cta(Color::BLACK) + "Black: ") << Cts(Color::DEFAULT)
                  << (blackMinutes < 10 ? "0" : "") << blackMinutes << ":"
                  << (blackSeconds < 10 ? "0" : "") << blackSeconds;

        // Restore the previous cursor position so user input stays in place.
        std::cout << "\033[u";  // ANSI escape code to restore cursor position

        std::cout.flush();


        // Sleep a short period to update the display continuously.
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



void ChessGame::Init()
{
    SetupPlayers();
    SetupTimers();
}

void ChessGame::EndGame(Player* winner, bool resign, bool notime)
{
    std::cout << "\n";
    PrintBox(
        winner->Name() + " has won!",
        " Game ended "
    );

    exit(0);
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
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

    Player* player = m_playerOnTurn->Color() == m_playerOne->Color() ? m_playerTwo : m_playerOne;

    oss << "Player " << player->Name() << " may make a move.\n"
        << Cta(m_playerOne->Color()) << m_playerOne->Name() << Cta(Color::DEFAULT)  << " has captured: " << Cta(m_playerOne->Color());

    for (auto capture : m_playerOne->GetCaptures())
        oss << capture->Symbol() << ' ';
    oss << '\n'<< Cta(m_playerTwo->Color()) << m_playerTwo->Name() << Cta(Color::DEFAULT)  << " has captured: " << Cta(m_playerTwo->Color());


    for (auto capture : m_playerTwo->GetCaptures())
        oss << capture->Symbol() << ' ';

    oss << Cta(Color::DEFAULT);

    PrintBox(oss.str(), " Game Stats ");

    m_board->Draw();

}

void ChessGame::StartTimer()
{
    // Record the current time as the start of the move.
    m_moveStartTime = std::chrono::steady_clock::now();
}

void ChessGame::StopTimer()
{
    // Get the current time and calculate the elapsed time in seconds.
    auto stopTime = std::chrono::steady_clock::now();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(stopTime - m_moveStartTime).count();

    // Depending on the player's color, subtract the elapsed time from the base timer and add the increment.
    if (m_playerOnTurn->Color() == Color::WHITE)
    {
        m_timerOne = (m_timerOne > elapsedSeconds) ? (m_timerOne - elapsedSeconds) : 0;
        m_timerOne += m_increment;  // Add the increment after the move.
    }
    else // Assuming Black.
    {
        m_timerTwo = (m_timerTwo > elapsedSeconds) ? (m_timerTwo - elapsedSeconds) : 0;
        m_timerTwo += m_increment;  // Add the increment after the move.
    }
}
