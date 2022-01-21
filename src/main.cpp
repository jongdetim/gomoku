#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"

#define PRINT(x) std::cout << x << std::endl

int main()
{
    // GUI gui;
    PrimitiveGui gui;
    Board board;
    
    board.play(gui);
    
    return 0;
}
