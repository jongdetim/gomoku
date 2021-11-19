#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# define WINCONDITION	5
# define CAPTUREWIN		5

# define LEFT			-1
# define RIGHT			1
# define UP				-(BOARD_LENGTH)
# define DOWN			BOARD_LENGTH
# define DIAGUPR		-(BOARD_LENGTH - 1)
# define DIAGUPL		-(BOARD_LENGTH + 1)
# define DIAGDWNR		(BOARD_LENGTH + 1)
# define DIAGDWNL		(BOARD_LENGTH - 1)

class Board;

class BoardHeuristic
{
public:
	BoardHeuristic(void);
	~BoardHeuristic();

	bool				continue_game(const Board *board, int index, int player) const;
	bool				has_won(const Board *board, int index, int player) const;
	int					count_both_dir(const Board *board, int index, int player, int dir) const;
	bool				check_wincodition_all_dir(const Board *board, int index, int player) const;
	int					count_direction(const Board *board, int index, int player, int dir, int size) const;

private:

};

#endif