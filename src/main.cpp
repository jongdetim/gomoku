#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

#define PRINT(x) std::cout << x << std::endl

int main()
{
    Board board;

    place_pieces(board, PLAYER2, calc_index(8, 8), 3, HOR);
    place_pieces(board, PLAYER2, calc_index(8, 8), 3, VER);
    place_pieces(board, PLAYER2, calc_index(8, 8), 3, DIAG1);
    place_pieces(board, PLAYER2, calc_index(8, 8), 3, DIAG2);

    place_pieces(board, PLAYER2, calc_index(8, 8), 3, -HOR);
    place_pieces(board, PLAYER2, calc_index(8, 8), 3, -VER);
    place_pieces(board, PLAYER2, calc_index(8, 8), 3, -DIAG1);
    place_pieces(board, PLAYER2, calc_index(8, 8), 3, -DIAG2);

    place_pieces(board, PLAYER1, calc_index(8, 8), 4, HOR);
    place_pieces(board, PLAYER1, calc_index(8, 8), 4, VER);
    place_pieces(board, PLAYER1, calc_index(8, 8), 4, DIAG1);
    place_pieces(board, PLAYER1, calc_index(8, 8), 4, DIAG2);

    place_pieces(board, PLAYER1, calc_index(8, 8), 4, -HOR);
    place_pieces(board, PLAYER1, calc_index(8, 8), 4, -VER);
    place_pieces(board, PLAYER1, calc_index(8, 8), 4, -DIAG1);
    place_pieces(board, PLAYER1, calc_index(8, 8), 4, -DIAG2);
    
    board.remove(calc_index(8, 8));

    PRINT("Stones played");
    PRINT(board.stones_played);
    board.print();


    
    PRINT("--------------");
    board.place(calc_index(8, 8), PLAYER1);
    PRINT("Stones played");
    PRINT(board.stones_played);
    board.show_last_move();

    return 0;
}
