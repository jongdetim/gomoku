#ifndef GUIBOARD_HPP
# define GUIBOARD_HPP

# include "Board.hpp"
# include "GuiPlayer.hpp"

# define GUIBOARD this->guiboard.get_board()

class GuiBoard
{
public:
	GuiBoard(void);
	GuiBoard(Board board);
	GuiBoard(const GuiBoard &rhs);

	GuiPlayer	players[2];

	GuiPlayer	current_player(void) const;
	Board		&get_board(void);
	
	GuiBoard	&operator=(GuiBoard const &rhs);
private:
	Board		board;
};

#endif