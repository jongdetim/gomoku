#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# include "gomoku.hpp"

class Board;

class Heuristic
{
public:
	Heuristic(void);
	~Heuristic();

	bool				continue_game(const Board *board, int index, int player) const;
	bool				has_won(const Board *board, int index, int player) const;
	int					count_both_dir(const Board *board, int index, int player, int dir) const;
	bool				check_wincodition_all_dir(const Board *board, int index, int player) const;
	int					count_direction(const Board *board, int index, int player, int dir, int size) const;
	int					calc_heuristic(Board &node);
	int					eight_directions_heuristic(int index, std::bitset<BOARDSIZE> &checked_indices, int player, Board &node);
	int					get_heuristic_last_move(Board *board);

private:

};

#endif