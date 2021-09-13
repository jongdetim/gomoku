#include "Board.hpp"
#include "algorithm.hpp"

int main()
{

    int move_index = 144;

    Board node;
    node.place(move_index, PLAYER1);
    // node.place(100, PLAYER2);
    node.print();

    std::vector<int> filled_positions;
    negamax(node, 2, -std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), 1, filled_positions);

    return 0;
}
