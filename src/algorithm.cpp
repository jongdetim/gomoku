#include "algorithm.hpp"

int TOTAL_LEAVES = 0;
int TOTAL_NODES = 0;

bool TranspositionTable::lookup(Board &node, TableEntry &tt_entry)
{
    std::unordered_map<std::bitset<MASKSIZE>, TableEntry>::const_iterator get;

    get = t_table.find(node.state);
    if (get == t_table.end())
        return false;
    else
    {
        tt_entry = get->second;
        // std::cout << "i've seen this state before :)))" << std::endl;
        return true;
    }
}

void TranspositionTable::insert(Board &node, TableEntry &tt_entry)
{
    t_table.insert({node.state, tt_entry});
}

size_t TranspositionTable::size()
{
    return t_table.size();
}

void order_moves(std::vector<Board> *nodes)
{
    ;
}

int negamax(Board node, int depth, int alpha, int beta, int color, std::vector<int> filled_positions, TranspositionTable &t_table)
{
    TableEntry tt_entry;
    bool found = false;
    int alpha_orig = alpha;
    int value = -std::numeric_limits<int>::max();
    bool is_finished;

    // (* Transposition Table Lookup; node is the lookup key for tt_entry *)
    found = t_table.lookup(node, tt_entry);
    if (found and tt_entry.depth >= depth)
    {
        if (tt_entry.flag == EXACT)
        {
            // std::cout << "EXACT FLAG FOUND IN TABLE " << std::endl;
            return tt_entry.value;
        }
        else if (tt_entry.flag == LOWERBOUND)
        {
            // std::cout << "LOWERBOUND FLAG FOUND IN TABLE " << std::endl;
            alpha = std::max(alpha, tt_entry.value);
        }
        else if (tt_entry.flag == UPPERBOUND)
        {
            // std::cout << "UPPERBOUND FLAG FOUND IN TABLE " << std::endl;
            beta = std::min(beta, tt_entry.value);
        }
        if (alpha >= beta)
            return tt_entry.value;
    }
    is_finished = node.is_game_finished();
    if (depth == 0 || is_finished)
    {
        TOTAL_LEAVES += 1;
        // node.print();
        return (color * node.get_heuristic());
    }

    filled_positions.push_back(node.last_move);
    // std::cout << "last_move: " << node.last_move << std::endl;
    std::vector<Board> child_nodes = node.generate_children(filled_positions, color);
    order_moves(&child_nodes);
    
    for (Board child : child_nodes)
    {
        TOTAL_NODES += 1;
        value = std::max(value, -negamax(child, depth - 1, -beta, -alpha, -color, filled_positions, t_table));
        alpha = std::max(alpha, value);
        if (alpha >= beta)
        {
            // std::cout << "branch pruned!" << std::endl;
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
    t_table.insert(node, tt_entry);

    return value;
}
