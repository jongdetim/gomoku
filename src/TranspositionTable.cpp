#include "TranspositionTable.hpp"

bool		TranspositionTable::lookup(Board &node, TableEntry &tt_entry)
{
	std::unordered_map<BITBOARD, TableEntry>::const_iterator get;

	get = t_table.find(node.get_state());
	if (get == t_table.end())
		return false;
	else
	{
		tt_entry = get->second;
		// std::cout << "i've seen this state before :)))" << std::endl;
		return true;
	}
}

void		TranspositionTable::insert(Board &node, TableEntry &tt_entry)
{
	t_table.insert({node.get_state(), tt_entry});
}

void		TranspositionTable::update(Board &node, int value)
{
	t_table[node.get_state()].value = value;
}

size_t		TranspositionTable::size()
{
	return t_table.size();
}
