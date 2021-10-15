#ifndef BOARDHEURISTIC_HPP
# define BOARDHEURISTIC_HPP

class Board;

class BoardHeuristic
{
public:
	BoardHeuristic(Board *board);
	~BoardHeuristic();

	bool				check_win(void) const;
	int					count_diag_down(void) const;
	int					count_diag_up(void) const;
	int					count_ver(void) const;
	int					count_hor(void) const;
	static int			go_down(Board *board, int index, int player, int size);
	static int			go_up(Board *board, int index, int player, int size);
	static int			go_left(Board *board, int index, int player, int size);
	static int			go_right(Board *board, int index, int player, int size);
	static int			diag_upR(Board *board, int index, int player, int size);
	static int			diag_downL(Board *board, int index, int player, int size);
	static int			diag_upL(Board *board, int index, int player, int size);
	static int			diag_downR(Board *board, int index, int player, int size);

private:
	BoardHeuristic(void);
	
	Board				*board;
};

#endif