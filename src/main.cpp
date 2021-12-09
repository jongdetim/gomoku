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

void    test(Board board)
{
    PRINT("-------------");
    PRINT("");

    board.current_player->print();
    PRINT(board.current_player);
    PRINT("");
    board.player1.print();
    PRINT(&board.player1);
    PRINT("");
    board.player2.print();
    PRINT(&board.player2);
    board.show_move();
}

int main()
{
    Board board;

    // board.play(&random_board_int);//, &random_board_int);

    return 0;
}
