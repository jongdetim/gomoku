/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

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
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "algorithm.hpp"

bool		TranspositionTable::retrieve(Board &node, TableEntry &tt_entry)
{
	std::unordered_map<Board, TableEntry, MyHashFunction>::iterator get;

	get = t_table.find(node);
	if (get == t_table.end())
		return false;
	else
	{
		tt_entry = get->second;
		return true;
	}
}

bool		TranspositionTable::contains(Board &node)
{
	std::unordered_map<Board, TableEntry, MyHashFunction>::iterator get;

	get = t_table.find(node);
	return !(get == t_table.end());
}

void		TranspositionTable::insert(const Board &node, TableEntry &tt_entry)
{
	t_table.insert({node, tt_entry});
}

void		TranspositionTable::update(Board &node, TableEntry &tt_entry)
{
	t_table[node] = tt_entry;
}

size_t		TranspositionTable::size()
{
	return t_table.size();
}

size_t MyHashFunction::operator()(const Board &Key) const
{
	// return default_hash(Key.get_state());
	return Key.zobrist_hash;
}

uint64_t MyHashFunction::hash(BITBOARD board) const
{
    uint64_t result = 0;
	uint64_t mask = 1;

    for (int chunk = 0; chunk < 12; chunk++)
    {
        for (int i = 0; i < 64; i++)
		{
			if (board[i])
				result ^= mask;
			mask <<= 1;
		}
		result = (result ^ (result >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		// result = (result ^ (result >> 27)) * UINT64_C(0x94d049bb133111eb);
		// result = result ^ (result >> 31);
        board >>= 64;
		mask = 1;
    }
    return result;
}

size_t MyHashFunction::hash32(BITBOARD board) const
{
    size_t result = 0;
	uint32_t mask = 1;

    for (int chunk = 0; chunk < 23; chunk++)
    {
        for (int i = 0; i < 32; i++)
		{
			if (board[i])
				result ^= mask;
			mask <<= 1;
		}
		if (chunk % 2 == 0)
			result = (result ^ (result >> 15)) * UINT64_C(0xbf58476d1ce4e5b9);
		// result = (result ^ (result >> 27)) * UINT64_C(0x94d049bb133111eb);
		// result = result ^ (result >> 31);
        board >>= 32;
		mask = 1;
    }
    return result;
}

uint64_t MyHashFunction::hash_diff(BITBOARD board) const
{
    uint64_t result = 0;

    for (int chunk = 0; chunk < 12; chunk++)
    {
		std::bitset<64> mask(board.to_string());
		result ^= mask.to_ullong();
		result = (result ^ (result >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
		// result = result ^ (result >> 31);
        board >>= 64;
    }
    return result;
}

uint64_t MyHashFunction::default_hash(BITBOARD board) const
{
	std::hash<BITBOARD> dhash;
    return dhash(board);
}
