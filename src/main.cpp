#include "Board.hpp"
#include "GUI.hpp"
#include "PrimitiveGui.hpp"
#include "gomoku.hpp"
#include "heuristic.hpp"
#include "TranspositionTable.hpp"
#include "misc.hpp"
#include "algorithm.hpp"

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
    RandomAi ai;
    // PrimitiveGui gui(&ai);
    GUI gui(&ai);

    Board board;
    // GuiBoard guiboard(board);
    board.play(gui);
}
