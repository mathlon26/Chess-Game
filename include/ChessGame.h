#ifndef CHESS_GAME__H
#define CHESS_GAME__H

#include "CanvasApp.h"
#include "TwoPlayerGame.h"

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

    Move& GetPlayerMove();
private:
};

#endif // CHESS_GAME__H