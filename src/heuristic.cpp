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
	return ((i / MASK_LENGTH) + pattern_height) > BOARDSIZE;
}

inline bool 			is_at_edge(int i, int pattern_width)
{
	return (i != 0 and (i + (pattern_width)) % MASK_LENGTH == 0);
}

inline bool				pattern_found(Board state, std::bitset<MASKSIZE> pattern)
{
	return ((state & pattern) == pattern);
}

int						find_pattern(Board &state, int player, std::bitset<MASKSIZE> &pattern, t_size pattern_size, bool verbose)
{
	assert(pattern_size.width <= BOARDSIZE and pattern_size.height <= BOARDSIZE);
	pattern_size.width <<= 1;
	int found = 0;
	pattern = player == PLAYER1 ? pattern : pattern << 1;
	for (int i = 0; i < MASKSIZE; i+=2)
	{
		if (is_out_of_bounds(i, pattern_size.height))
			break ;
		if(pattern_found(state, (pattern<<i)))
		{
			if (verbose)
				print_bitmap(pattern<<i);
			found++;
		}
		if (is_at_edge(i, pattern_size.width))
			i += (pattern_size.width - 2);
	}
	return found;
}

inline int				get_end_bit(std::bitset<MASKSIZE> &pattern, t_size pattern_size)
{
	return (pattern[(pattern_size.width * pattern_size.height) - 1] == false);
}

int						g_points[5]{0,ROW2,ROW3,ROW4,ROW5}; // Just to test

int						get_heuristic(Board &state, std::bitset<MASKSIZE> &pattern, t_size pattern_size)
{
	assert(pattern_size.width <= BOARDSIZE and pattern_size.height <= BOARDSIZE);
	int points = g_points[std::max(pattern_size.width, pattern_size.height) - 1];
	int score = 0;
	int player = -1;
	pattern_size.width <<= 1;
	pattern_size.width -= get_end_bit(pattern, pattern_size);
	for (int i = 0; i < MASKSIZE; i++)
	{
		if (is_out_of_bounds(i, pattern_size.height))
			break ;
		if(pattern_found(state, (pattern<<i)))
			score += player * points;
		if (is_at_edge(i, pattern_size.width))
			i += (pattern_size.width - 1);
		player *= -1;
	}
	return score;
}

int						calc_heuristic(Board &state)
{
	int						sizes[4]{2,3,4,5};
	t_size					size_pattern;
	std::bitset<MASKSIZE>	pattern;
	int						score = 0;

	for (auto size : sizes)
	{
		pattern = create_ver(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);

		pattern = create_hor(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);

		pattern = create_diag_left(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);

		pattern = create_diag_right(size, size_pattern);
		score += get_heuristic(state, pattern, size_pattern);
	}
	return score;
}
