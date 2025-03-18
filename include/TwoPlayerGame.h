#ifndef TWO_PLAYER_GAME__H
#define TWO_PLAYER_GAME__H

#include "Player.h"

class TwoPlayerGame
{
public:
    TwoPlayerGame();
    ~TwoPlayerGame();

    Player* GetPlayerOnTurn();
    Player* SwitchTurn();

    virtual void SetupPlayers() = 0;
protected:
    Player* m_playerOne{nullptr};
    Player* m_playerTwo{nullptr};
    Player* m_playerOnTurn{nullptr};
};

#endif // TWO_PLAYER_GAME__H