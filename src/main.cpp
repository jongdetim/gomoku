#include "Board.hpp"
#include "algorithm.hpp"
#include "misc.hpp"
#include "GUI.hpp"

#define PRINT(x) std::cout << x << std::endl

int main()
{
    GUI gui(big);
    Board board;

    if (!gui.initiate_GUI("Gomoku"))
        return 1;
    
    gui.game(board);
    
    return 0;
}
