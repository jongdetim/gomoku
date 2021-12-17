#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# include "gomoku.hpp"

class Board;

enum Pattern {none, closed2, open2, closed3, open3, closed4, open4, five};
const char * const PatternNames[8] = {"none", "closed2", "open2", "closed3", "open3", "closed4", "open4", "five"};

const std::map<uint8_t, Pattern> SUBPATTERNS_5 = {	{0b00011111, five},
													{0b00001111, closed4}, {0b00011110, closed4}, {0b00010111, closed4}, {0b00011011, closed4}, {0b00011101, closed4},
													// {0b00010101, closed3}, {0b00010110, closed3}, {0b00001011, closed3},
													{0b00001110, open3}	 };

const std::map<uint8_t, Pattern> SUBPATTERNS_6 = {	{0b00011110, open4},
													{0b00011010, open3}, {0b00010110, open3}  };

typedef struct s_pattern
{
	int left_right[2];
	int count;
	int space;
    int length;
    unsigned char pattern;
}   t_pattern;

typedef struct s_subpattern
{
    int length;
    unsigned char pattern;
}   t_subpattern;

class Heuristic
{
public:
	int		score = 0;
	uint8_t patterns[8] = {0};

	Heuristic(void);
	~Heuristic();

	bool				continue_game(const Board *board, int index, int player) const;
	bool				has_won(const Board *board, int index, int player) const;
	int					count_both_dir(const Board *board, int index, int player, int dir) const;
	int					count_both_dir(const Board *board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices) const;
	bool				check_wincodition_all_dir(const Board *board, int index, int player) const;
	int					count_direction(const Board *board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices) const;
	int					count_direction(const Board *board, int index, int player, int dir, int size) const;
	int					calc_heuristic(Board *board);
	int					eight_directions_heuristic(Board *node, int index, std::bitset<BOARDSIZE> &checked_indices, int player);
	int					get_heuristic_last_move(Board *board);
	int					determine_score(int count, int gaps, int open) const;
	bool				get_direction(const Board *board, int move, int direction, int player) const;
	
private:

};

#endif