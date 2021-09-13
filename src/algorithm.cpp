#include <vector>
#include <limits>
#include <iostream>
#include <algorithm>

#define EXACT 0
#define LOWERBOUND 1
#define UPPERBOUND 2

class Board
{
    public:
        int x = 0;
        int game_finished;
        int heuristic;
};

class TableEntry
{
    public:
        int value;
        int depth;
        int flag;
};

std::vector<Board> generate_moves(Board node)
{
    std::vector<Board> moves;

    return moves;
}

bool transposition_table_lookup(Board node, TableEntry *tt_entry)
{
    return true;
}

void transposition_table_store(Board node, TableEntry *tt_entry)
{
    
}

void order_moves(std::vector<Board> *nodes)
{
    ;
}

int negamax(Board node, int depth, int alpha, int beta, int color)
{
    TableEntry tt_entry;
    bool found = false;
    int alpha_orig = alpha;
    int value = -std::numeric_limits<int>::max();

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
    if (depth == 0 || node.game_finished)
        return (color * node.heuristic);

    std::vector<Board> child_nodes = generate_moves(node);
    order_moves(&child_nodes);
    
    for (Board child : child_nodes)
    {
        value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, -color));
        alpha = std::max(alpha, value);
        if (alpha >= beta)
            break;
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
    transposition_table_store(node, &tt_entry);

    return value;
}
