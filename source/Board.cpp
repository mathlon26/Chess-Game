#include "../include/Board.h"
#include "../dependencies/Style.hpp"
#include <iostream>
#include <sstream>
#include <string>

Board::Board()
    :
    m_board{}
{
}

Board::~Board()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            delete m_board[i][j];
}

void Board::Draw() const
{
    std::ostringstream oss;

    oss << "   a  b  c  d  e  f  g  h\n";

    int count{0};

    for (int i = 0; i < 8; ++i)  {
        oss << 8 - i << " ";

        for (int j = 0; j < 8; ++j) {
            if (count++ % 2 == 0)
                oss << "\033[44m";
            else
                oss << "\033[42m";

            if (m_board[i][j] == nullptr)
                oss << "   ";
            else
            {
                oss << Style::Cta(m_board[i][j]->GetColor());
                oss << ' ' << m_board[i][j]->GetSymbol() << ' ';
            }
            oss << "\033[0m";
        }

        oss << ' ' << 8 - i << '\n';

        count++;
    }
    oss << "\033[0m   a  b  c  d  e  f  g  h"<< std::endl;

    Style::PrintBox(oss.str(), " Board ");
}