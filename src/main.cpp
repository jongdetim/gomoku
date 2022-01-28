#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"

#define PRINT(x) std::cout << x << std::endl

class RandomAi : public IAi
{
public:
	int	calculate(Board &board)
    {
        return misc::random(0, (BOARDSIZE - 1));
    }
};

int main()
{
    // PrimitiveGui gui(NULL);
    // RandomAi ai;
    // GUI gui(&ai);

    GUI gui(NULL);
    Board board;
    
    board.play(gui);

    return 0;
}
