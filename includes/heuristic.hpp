#ifndef HEURISTIC_HPP
# define HEURISTIC_HPP

# include <cmath>
# include <bitset>
# include <iostream>
# include <cassert>
# include <algorithm>
# include "Board.hpp"

# define ROW5 1000
# define ROW4 100
# define ROW3 10
# define ROW2 1

int				calc_heuristic(Board &state);
// int				calc_heuristic_tim_from_parent(std::vector<int> filled_positions, Board &node);
int				calc_heuristic_tim(Board &node);

#endif
