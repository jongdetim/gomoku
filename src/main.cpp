# include "gomoku.hpp"
# include "Heuristic.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"
# include "algorithm.hpp"

#define PRINT(x) std::cout << x << std::endl

int main()
{
    Board board;

    board.play();
    
    return 0;
}
