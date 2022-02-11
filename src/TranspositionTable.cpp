# include "Board.hpp"
# include "TranspositionTable.hpp"


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
	return default_hash(Key.get_state());
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
		// result *= (chunk + 1) * ;
		// cout << result << "   " << endl;
        board >>= 64;
		mask = 1;
		// cout << board << endl;
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
		// result *= (chunk + 1) * ;
		// cout << result << "   " << endl;
        board >>= 32;
		mask = 1;
		// cout << board << endl;
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
		// result *= (chunk + 1) * ;
		// cout << result << "   " << endl;
        board >>= 64;
		// cout << board << endl;
    }
    return result;
}

uint64_t MyHashFunction::default_hash(BITBOARD board) const
{
	std::hash<BITBOARD> dhash;
    return dhash(board);
}

uint64_t MyHashFunction::dumb_hash(BITBOARD board) const
{
    return 0;
}
