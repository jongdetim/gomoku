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

    for (int i = 0; i < 3; i++)
    {
        board.place(random(0, BOARDSIZE - 1, 1));
        board.next_player();
        board.place(random(0, BOARDSIZE - 1, 1));
        board.next_player();
    }


    PRINT("");
    board.print_values();
    PRINT("");
    // board.show_move();
    PRINT("------------------------");

    Board copy = board;
    copy.print_values();
    PRINT("");
    // copy.show_move();
    return 0;
}
