#ifndef TRANSPOSITION_TABLE_HPP
#define TRANSPOSITION_TABLE_HPP

#include "board.hpp"
#include "gomoku.hpp"

class MyHashFunction
{
private:
    // hash<BITBOARD> mHash;
public:
    uint64_t hash(BITBOARD board) const;
    size_t hash32(BITBOARD board) const;
    uint64_t hash_diff(BITBOARD board) const;
    uint64_t default_hash(BITBOARD board) const;
    uint64_t dumb_hash(BITBOARD board) const;
    size_t operator()(const Board &Key) const;
};

// class TableEntry
// {
// public:
//     int value;
//     int depth;
//     int flag;
//     bool game_finished;
//     int best_move;
// };

class TranspositionTable
{
private:

public:
    std::unordered_map<Board, TableEntry, MyHashFunction> t_table;
    // std::unordered_map<BITBOARD, TableEntry> t_table;
    bool lookup(Board &node, TableEntry &tt_entry);
    void insert(const Board &node, TableEntry &tt_entry);
    void update(Board &node, int value);
    size_t size();
};

#endif
