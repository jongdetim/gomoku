# include "gomoku.hpp"
# include "Heuristic.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"
# include "algorithm.hpp"

#define PRINT(x) std::cout << x << std::endl

int random_board_int(void)
{
    return random(0, BOARDSIZE-1);
}

int main()
{
    Board board;

    board.play(&random_board_int);//, &random_board_int);
    
    return 0;
}
