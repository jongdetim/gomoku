#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "gomoku.hpp"
#include "algorithm.hpp"

class Board;

class MyHashFunction
{
public:
    uint64_t hash(BITBOARD board) const;
    size_t hash32(BITBOARD board) const;
    uint64_t hash_diff(BITBOARD board) const;
    uint64_t default_hash(BITBOARD board) const;
    size_t operator()(const Board &Key) const;
};

class TableEntry
{
public:
    int                 depth;
    int                 flag;
    t_search_results    results;
};

class TranspositionTable
{
private:

public:
    std::unordered_map<Board, TableEntry, MyHashFunction> t_table;
    bool contains(Board &node);
    bool retrieve(Board &node, TableEntry &tt_entry);
    void insert(const Board &node, TableEntry &tt_entry);
    void update(Board &node, TableEntry &tt_entry);

    size_t size();
};

#endif
