#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

void create_star(Board &board, int index, int size, int player)
{
    for (auto n : NEIGHBOURS)
    {
        place_pieces(board, player, index, size, n);
    }
}

int main()
{
    Board board;
    BoardHeuristic heuristic;

    // board = create_random_board(17);
    int index = calc_index(3, 16);
    create_star(board, index, 5, PLAYER1);

    board.remove(index);
    board.place(index, PLAYER1);
    board.show_last_move();

    std::cout << std::endl;
    heuristic.count_diag_down(&board);
    heuristic.count_diag_up(&board);
    heuristic.count_hor(&board);
    heuristic.count_ver(&board);
    std::cout << std::endl;


    std::cout << std::endl;

    return 0;
}
