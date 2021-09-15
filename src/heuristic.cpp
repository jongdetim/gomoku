#include "heuristic.hpp"
#include "misc.hpp"

std::bitset<MASKSIZE>	create_ver(int size, t_size &pattern_size)
{
	std::bitset<MASKSIZE> pattern{1};
	pattern_size.height = size; pattern_size.width = 1;

	for (int i = 1; i < size; i++)
	{
		pattern <<= MASK_LENGTH;
		pattern ^= 1;
	}
	return pattern;
}

std::bitset<MASKSIZE>	create_hor(int size, t_size &pattern_size)
{
	std::bitset<MASKSIZE> pattern{1};
	pattern_size.height = 1; pattern_size.width = size;

	for (int i = 1; i < size; i++)
	{
		pattern <<= 2;
		pattern ^= 1;
	}
		
	return pattern;
}

std::bitset<MASKSIZE>	create_diag_left(int size, t_size &pattern_size)
{
	std::bitset<MASKSIZE> pattern{1};
	pattern_size.height = size; pattern_size.width = size;

	for (int i = 1; i < size; i++)
	{
		pattern <<= (MASK_LENGTH + 2);
		pattern ^= 1;
	}
		
	return pattern;
}

std::bitset<MASKSIZE>	create_diag_right(int size, t_size &pattern_size)
{
	std::bitset<MASKSIZE> pattern{1};
	pattern_size.height = size; pattern_size.width = size;

	for (int i = 1; i < size; i++)
	{
		pattern <<= MASK_LENGTH;
		pattern ^= (1 << (i << 1));
	}
		
	return pattern;
}

inline bool				is_out_of_bounds(int i, int pattern_height)
{
	return ((i / MASK_LENGTH) + (pattern_height >> 1)) > BOARDSIZE;
}

inline bool 			is_at_edge(int i, int pattern_width)
{
	return (i != 0 and (i + pattern_width) % MASK_LENGTH == 0);
}

int						find_pattern(Board state, int player, std::bitset<MASKSIZE> pattern, t_size pattern_size)
{
	assert(pattern_size.width <= BOARDSIZE and pattern_size.height <= BOARDSIZE);
	pattern_size.width <<= 1; pattern_size.height <<= 1;
	int found = 0;
	pattern = player == PLAYER1 ? pattern : pattern << 1;
	for (int i = 0; i < MASKSIZE; i+=2)
	{
		if (is_out_of_bounds(i, pattern_size.height))
			break ;
		if((state & (pattern<<i)) == (pattern<<i))
		{
			// print_bitmap(pattern<<i);
			found++;
		}
		if (is_at_edge(i, pattern_size.width))
			i += (pattern_size.width - 2);
	}
	return found;
}
