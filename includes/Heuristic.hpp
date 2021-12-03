#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# include "gomoku.hpp"

class Board;
class Player;

class Heuristic
{
public:
	Heuristic(void);
	~Heuristic();

	bool				continue_game(const Board *board, Player &player) const;
	bool				has_won(const Board *board, Player &player) const;
	int					count_both_dir(const Board *board, int index, int player, int dir) const;
	int					count_both_dir(const Board *board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices) const;
	bool				check_wincodition_all_dir(const Board *board, int index, int player) const;
	int					count_direction(const Board *board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices) const;
	int					count_direction(const Board *board, int index, int player, int dir, int size) const;
	int					calc_heuristic(Board *board);
	int					eight_directions_heuristic(Board *node, int index, std::bitset<BOARDSIZE> &checked_indices, int player);
	int					get_heuristic_from_player(Board *board, Player &player);
	int					determine_score(int count, int gaps, int open) const;
	bool				get_direction(const Board *board, int move, int direction, int player) const;
	
private:

};

#endif