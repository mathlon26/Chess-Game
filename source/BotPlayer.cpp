#include "../include/BotPlayer.h"
#include "../include/Board.h"


BotPlayer::BotPlayer(PlayerColor color)
    :
    Player{color}
{
}

BotPlayer::~BotPlayer()
{
    for (Piece* piece : m_captures)
        delete piece;
}

std::string BotPlayer::Name() const
{
    return Style::Cts(m_color) + " (Bot)";
}

std::string BotPlayer::GetInput(const Board* const board) const
{
    std::vector<Move> moves = board->GetPossibleMoves(m_color);
    std::vector<Move> validMoves{};

    for (const auto& move : moves)
    {
        if (std::find(m_cache.begin(), m_cache.end(), move.toString()) == m_cache.end())
        {
            validMoves.push_back(move);
        }
    }
    



    // If no moves are available, return a "resign" command.
    if (validMoves.empty())
        return "resign";

    // Initialize random engine with a random seed
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, validMoves.size() - 1);
    int index = dis(gen);

    Move chosenMove = validMoves[index];


    return chosenMove.toString();
}

bool BotPlayer::IsBot()
{
    return true;
}

void BotPlayer::AddCache(std::string moveStr)
{
    m_cache.insert(moveStr);
}

void BotPlayer::EmptyCache()
{
    m_cache.clear();
}