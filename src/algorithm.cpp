#include "algorithm.hpp"

#define EXACT 0
#define LOWERBOUND 1
#define UPPERBOUND 2

bool transposition_table_lookup(Board node, TableEntry *tt_entry)
{
    return false;
}

void transposition_table_store(Board node, TableEntry *tt_entry)
{
    ;
}

void order_moves(std::vector<Board> *nodes)
{
    ;
}

int negamax(Board node, int depth, int alpha, int beta, int color, std::vector<int> filled_positions)
{
    TableEntry tt_entry;
    bool found = false;
    int alpha_orig = alpha;
    int value = -std::numeric_limits<int>::max();
    bool is_finished;

    // (* Transposition Table Lookup; node is the lookup key for tt_entry *)
    found = transposition_table_lookup(node, &tt_entry);
    if (found and tt_entry.depth >= depth)
    {
        if (tt_entry.flag == EXACT)
            return tt_entry.value;
        else if (tt_entry.flag == LOWERBOUND)
            alpha = std::max(alpha, tt_entry.value);
        else if (tt_entry.flag == UPPERBOUND)
            beta = std::min(beta, tt_entry.value);

        if (alpha >= beta)
            return tt_entry.value;
    }
    is_finished = node.is_game_finished();
    if (depth == 0 || is_finished)
    {
        // node.print();
        return (color * node.get_heuristic());
    }

    filled_positions.push_back(node.last_move);
    std::cout << "last_move: " << node.last_move << std::endl;
    std::vector<Board> child_nodes = node.generate_children(filled_positions, color);
    order_moves(&child_nodes);
    
    for (Board child : child_nodes)
    {
        value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, -color, filled_positions));
        alpha = std::max(alpha, value);
        if (alpha >= beta)
        {
            std::cout << "branch pruned!" << std::endl;
            break;
        }
    }

    // (* Transposition Table Store; node is the lookup key for tt_entry *)
    tt_entry.value = value;
    if (value <= alpha_orig)
        tt_entry.flag = UPPERBOUND;
    else if (value >= beta)
        tt_entry.flag = LOWERBOUND;
    else
        tt_entry.flag = EXACT;
    tt_entry.depth = depth;
    tt_entry.game_finished = is_finished;
    transposition_table_store(node, &tt_entry);

    return value;
}
