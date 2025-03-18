#ifndef CHESS_GAME__H
#define CHESS_GAME__H

#include "CanvasApp.h"
#include "TwoPlayerGame.h"
#include "Board.h"

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

    Move GetPlayerMove();
private:
    Board* m_board;
};

#endif // CHESS_GAME__H