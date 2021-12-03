# include "gomoku.hpp"
# include "Heuristic.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"
# include "algorithm.hpp"

#define PRINT(x) std::cout << x << std::endl


// # include <unistd.h>
int random_board_int(void)
{
    // usleep(1500000);
    return random(0, BOARDSIZE-1);
}

int main()
{
    Board board;

    board.play(&random_board_int, &random_board_int);
    
    return 0;
}
