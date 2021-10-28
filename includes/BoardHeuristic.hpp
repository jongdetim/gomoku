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

	bool				check_win(const Board *board) const;
	int					count_diag_down(const Board *board) const;
	int					count_diag_up(const Board *board) const;
	int					count_ver(const Board *board) const;
	int					count_hor(const Board *board) const;
	short				get_pattern(int index, int player) const;
	int					count_direction(const Board *board, int index, int player, int size, int dir) const;

private:

};

#endif