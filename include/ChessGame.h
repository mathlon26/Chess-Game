#ifndef CHESS_GAME__H
#define CHESS_GAME__H

#include "CanvasApp.h"
#include "TwoPlayerGame.h"
#include "Board.h"
#include <chrono>
#include <atomic>
#include <thread>
#include <chrono>

class ChessGame : public CanvasApp, public TwoPlayerGame
{
public:
    ChessGame();
    ~ChessGame();

    void Run() override;
    void SetupPlayers() override;
    void Init() override;
    void Update() override;
    void Draw() const override;

    void EndGame(Player* winner, bool resign=false, bool notime=false);

    void SetupTimers();
    void TimerThreadFunction();
    void StartTimer();
    void StopTimer();


    Move GetPlayerMove();
private:
    Board* m_board;
    size_t m_increment;
    size_t m_timerOne;
    size_t m_timerTwo;

    std::atomic<bool> m_timerActive{false};
    std::chrono::steady_clock::time_point m_moveStartTime;
    std::thread m_timerThread;
};

#endif // CHESS_GAME__H