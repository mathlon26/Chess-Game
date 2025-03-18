#ifndef CHESS_GAME__H
#define CHESS_GAME__H

#include "CanvasApp.h"

class ChessGame : public CanvasApp
{
public:
    ChessGame();
    ~ChessGame();

    void Init() override;
    void Update() override;
    void Draw() const override;
private:
};

#endif // CHESS_GAME__H