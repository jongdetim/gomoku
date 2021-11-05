#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

#define PRINT(x) std::cout << x << std::endl

void create_star(Board &board, int index, int size, int player)
{
    for (auto dir : DIRECTIONS)
    {
        place_pieces(board, player, index, size, dir);
    }
}

int main()
{
    Board board;
    BoardHeuristic heuristic;

    int index = calc_index(3, 16);
    create_star(board, index, 3, PLAYER1);
    create_star(board, index, 4, PLAYER2);
    board.remove(index);


    PRINT("Stones played");
    PRINT(board.get_stones_played());
    board.print();
    PRINT("--------------");

    board.place(index, PLAYER2);
    PRINT("Stones played");
    PRINT(board.get_stones_played());
    board.show_last_move();
    
    PRINT("--------------");

    index = calc_index(8, 6);
    create_star(board, index, 3, PLAYER2);
    create_star(board, index, 4, PLAYER1);
    board.remove(index);


    PRINT("Stones played");
    PRINT(board.get_stones_played());
    board.print();
    PRINT("--------------");

    board.place(index, PLAYER1);
    PRINT("Stones played");
    PRINT(board.get_stones_played());
    board.show_last_move();
    
    std::cout << std::endl;

    return 0;
}
