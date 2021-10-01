#include "Board.hpp"
#include "algorithm.hpp"
// #include "TranspositionTable.hpp"
#include "misc.hpp"

#define PRINT(x) std::cout << x << std::endl
#define PRINTENDL std::cout << std::endl

int			get_hor_pattern(Board &board, int start_index, int player);
int		    get_heuristic_last_move(Board &board);
#include <vector>
#include <algorithm>

int     main()
{
    Board board;
    int index, player;

    // for (int i = 0; i < 10; i++)
    // {
    //     board = create_random_board(i);
    //     PRINT("============");
    //     int points = get_heuristic_last_move(board);
    //     std::cout << "--------------------------- +" << std::endl;
    //     std::cout << "points:              " << points << std::endl;
    //     board.show_last_move();
    //     PRINT("============");
    // }

    // index = calc_index(10, 5);
    // place_pieces(board, PLAYER1, index, 4, HOR);
    // board.remove(index+2);
    // board.remove(index+1);
    // board.place(index+1, PLAYER1);
    // board.show_last_move();
    // std::bitset<10> pattern{(unsigned long long)get_hor_pattern(board, index+4, PLAYER1)};
    // PRINT(pattern);

    board = create_random_board();
    board.show_last_move();
    std::bitset<10> pattern{(unsigned long long)get_hor_pattern(board, index+4, PLAYER1)};
    PRINT(pattern);

    return 0;
}
