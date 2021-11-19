#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"

#define PRINT(x) std::cout << x << std::endl

int main()
{
    Board board;
    int index = calc_index(8, 6);
    int star_index = calc_index(3, 16); 

    create_star(board, star_index, 3, PLAYER1);
    create_star(board, star_index, 4, PLAYER2);
    
    board.remove(star_index);

    // board.place(star_index, PLAYER2);
    // std::cout << "captures : " << board.get_player_captures(PLAYER2) << std::endl;

    place_pieces(board, PLAYER1, index, 5, RIGHT);
    board.show_last_move();
    
    if (board.is_game_won())
        std::cout << "Won!!!" << std::endl;
    else
        std::cout << "Not Won :(" << std::endl;

    return 0;
}
