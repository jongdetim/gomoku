#ifndef TRANSPOSITION_TABLE_HPP
# define TRANSPOSITION_TABLE_HPP

# include "Board.hpp"
# include <unordered_map>

class TableEntry
{
    public:
        int value;
        int depth;
        int flag;
        bool game_finished;
};

class TranspositionTable
{
    private:
        

    public:
        std::unordered_map<BITBOARD, TableEntry> t_table;
        bool lookup(Board &node, TableEntry &tt_entry);
        void insert(Board &node, TableEntry &tt_entry);
        void update(Board &node, int value);
        size_t size();
};

#endif
