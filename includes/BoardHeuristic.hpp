#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

# define WINCONDITION 5
# define CAPTUREWIN 5

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
	static int			go_down(const Board *board, int index, int player, int size);
	static int			go_up(const Board *board, int index, int player, int size);
	static int			go_left(const Board *board, int index, int player, int size);
	static int			go_right(const Board *board, int index, int player, int size);
	static int			diag_upR(const Board *board, int index, int player, int size);
	static int			diag_downL(const Board *board, int index, int player, int size);
	static int			diag_upL(const Board *board, int index, int player, int size);
	static int			diag_downR(const Board *board, int index, int player, int size);
	short				get_pattern(int index, int player) const;

private:

};

#endif