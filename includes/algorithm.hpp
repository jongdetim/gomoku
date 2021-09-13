#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

#include "Board.hpp"

class TableEntry
{
    public:
        int value;
        int depth;
        int flag;
        bool game_finished;
};

int negamax(Board node, int depth, int alpha, int beta, int color);


#endif
