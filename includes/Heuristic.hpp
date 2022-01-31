#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# include "gomoku.hpp"

class Board;
class Player;

namespace heuristic
{
	int					count_both_dir(const Board &board, int index, int player, int dir);
	int					count_both_dir(const Board &board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices);
	int					count_direction(const Board &board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices);
	int					count_direction(const Board &board, int index, int player, int dir, int size);
	int					calc_heuristic(Board &board);
	int					eight_directions_heuristic(Board &board, int index, std::bitset<BOARDSIZE> &checked_indices, int player_id);
	int					get_heuristic_from_player(Board &board, const Player &player);
	int					determine_score(int count, int gaps, int open);
	bool				get_direction(const Board &board, int move, int direction, int player);
}

#endif