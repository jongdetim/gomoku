/* MIT License

Copyright (c) 2022 Flint Louis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
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
