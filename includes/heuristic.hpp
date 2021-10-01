#ifndef HEURISTIC_HPP
# define HEURISTIC_HPP

# include <cmath>
# include <bitset>
# include <iostream>
# include <cassert>
# include "Board.hpp"

# define ROW5 100000
# define ROW4 15000
# define ROW3 10000
# define ROW2 5000

typedef struct	s_size
{
	int width;
	int height;
}				t_size;

int				calc_heuristic(Board &state);

#endif
