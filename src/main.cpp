#include "Board.hpp"
#include "algorithm.hpp"
// #include "TranspositionTable.hpp"
#include "misc.hpp"

#define PRINT(x) std::cout << x << std::endl
#define PRINTENDL std::cout << std::endl

int			count_ver(Board &board, int index, int player);
int			count_hor(Board &board, int index, int player);
int		    count_diagR(Board &board, int index, int player);
int		    count_diagL(Board &board, int index, int player);

int			get_hor_pattern(Board &board, int start_index, int player);

int main()
{
    Board board;
    int index, player;

    PRINT("====VER=====");
    for ( int i = 0; i < 100; i++)
    {
        board = create_random_board(i);
        int ver = count_ver(board, board.last_move, board.get_last_player());
        if (ver > 2)
        {
            PRINT("============");
            board.show_last_move();
            PRINT(ver);
            PRINT("============");
        }
    }

    PRINT("====HOR=====");
    for ( int i = 0; i < 100; i++)
    {
        board = create_random_board(i);
        int hor = count_hor(board, board.last_move, board.get_last_player());
        if (hor > 2)
        {
            PRINT("============");
            board.show_last_move();
            PRINT(hor);
            PRINT("============");
        }
    }

    PRINT("====DIAG_R=====");
    for ( int i = 0; i < 5; i++)
    {
        board = create_random_board(i);
        int diagR = count_diagR(board, board.last_move, board.get_last_player());
        if (diagR > 2)
        {
            PRINT("============");
            board.show_last_move();
            PRINT(diagR);
            PRINT("============");
        }
    }

    PRINT("====DIAG_L=====");
    for ( int i = 0; i < 100; i++)
    {
        board = create_random_board(i);
        int diagL = count_diagL(board, board.last_move, board.get_last_player());
        if (diagL > 2)
        {
            PRINT("============");
            board.show_last_move();
            PRINT(diagL);
            PRINT("============");
        }
    }


    // index = calc_index(10, 5);
    // place_pieces(board, PLAYER1, index, 4, HOR);
    // board.remove(index+2);
    // board.remove(index+1);
    // board.place(index+1, PLAYER1);
    // board.show_last_move();
    // std::bitset<10> pattern{(unsigned long long)get_hor_pattern(board, index+4, PLAYER1)};
    // PRINT(pattern);

    // board.reset();
    // index = calc_index(17, 16);
    // board.place(index, PLAYER1);
    // count_diagR(board, board.last_move, board.get_last_player());
    // board.remove(index);
    // board.place(index, PLAYER1);
    // board.show_last_move();

    return 0;
}
