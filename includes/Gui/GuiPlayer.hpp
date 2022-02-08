#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"
# include "IAi.hpp"
# include "Board.hpp"

class GuiPlayer
{
public:

	GuiPlayer(void);
	GuiPlayer(t_player &player);

	std::string					name;
	IAi							*ai;

	void						print(void) const;
	bool						wincondition(void) const;
	int							captures(void) const;
	int							last_move(void) const;
	void						reset(void);

private:
	t_player					*player;
};

#endif