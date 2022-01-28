#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# include "gomoku.hpp"

class Board;

enum Pattern {none, closed2, open2, closed3, open3, closed4, open4, five};
const char * const PatternNames[8] = {"none", "closed2", "open2", "closed3", "open3", "closed4", "open4", "five"};
const int SCORES[] {0, 100, 900, 1000, 9000, 10000};
const std::map<uint8_t, Pattern> SUBPATTERNS_5 = {	{0b00011111, five},
													{0b00001111, closed4}, {0b00011110, closed4}, {0b00010111, closed4}, {0b00011011, closed4}, {0b00011101, closed4},
													// {0b00010101, closed3}, {0b00010110, closed3}, {0b00001011, closed3},
													{0b00001110, open3}	 };

const std::map<uint8_t, Pattern> SUBPATTERNS_6 = {	{0b00011110, open4},
													{0b00011010, open3}, {0b00010110, open3}  };

typedef struct s_pattern
{
	uint8_t left_right[2];
	uint8_t count;
	uint8_t space;
    uint8_t length;
    uint8_t pattern = 0;
}   t_pattern;

// typedef struct s_subpattern
// {
//     int length;
//     unsigned char pattern;
// }   t_subpattern;

class Heuristic
{
public:
	int		score = 0;
	uint8_t patterns[8] = {0};

	Heuristic(void);
	~Heuristic();

	static bool				continue_game(const Board *board, int index, int player);
	static bool				has_won(const Board *board, int index, int player);
	static int					count_both_dir(const Board *board, int index, int player, int dir);
	static int					count_both_dir(const Board *board, int index, int player, int dir, std::bitset<BOARDSIZE> &checked_indices);
	static bool				check_wincodition_all_dir(const Board *board, int index, int player);
	static int					count_direction(const Board *board, int index, int player, int dir, int size, std::bitset<BOARDSIZE> &checked_indices);
	static int					count_direction(const Board *board, int index, int player, int dir, int size);
	static int					calc_heuristic(Board *board);
	static int					eight_directions_heuristic(Board *node, int index, std::bitset<BOARDSIZE> &checked_indices, int player);
	static int					get_heuristic_last_move(Board *board);
	static int					determine_score(int count, int gaps, int open);
	static bool				get_direction(const Board *board, int move, int direction, int player);
	static void           		cutout_pattern(const Board &board, int move, int direction, int player, t_pattern &pat);
	static t_pattern			get_pattern_data(Board &board, int move, int direction, int player, std::bitset<BOARDSIZE> *checked_indices);
	static int					score_remaining_patterns(Board &board, int player);
	static int					evaluate_patterns(Board &board, int player);
	static Pattern				find_subpattern(t_pattern &pat, uint8_t length, const std::map<uint8_t, Pattern> &map);
	static Pattern				get_heuristic_data(Board &board, const int &move, const int &direction, const int &player, std::bitset<BOARDSIZE> *checked_indices);
	static void					get_heuristic_single(Board &board, int move, std::bitset<BOARDSIZE> *checked_indices);
	static int					get_heuristic_total(Board &board);
	static void  				reset_pattern_arrays(Board &board);

private:

};

#endif