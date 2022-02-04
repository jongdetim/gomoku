#ifndef GUIBOARD_HPP
# define GUIBOARD_HPP

# include "Board.hpp"
# include "GuiPlayer.hpp"

class GuiBoard
{
public:
	GuiBoard(void);
	GuiBoard(Board board);
	GuiBoard(const GuiBoard &rhs);

	GuiPlayer	players[2];

	void		show_last_move(void) const;
	void		show_move(int index) const;
	void		next_player(void);
	bool		place(int index);
	int			get_current_player(void) const;
	int			get_winner(void) const;

	bool		is_game_finished(void);
	bool		has_winner(void) const;
	bool		is_full(void) const;

	void		reset(void);
	void		random_player(void);

	GuiPlayer	current_player(void) const;
	Board		&get_board(void);
	
	GuiBoard	&operator=(GuiBoard const &rhs);
private:
	Board		board;
};

#endif