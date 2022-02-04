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
	// GuiPlayer(const Player &rhs);

	std::string					name;
	IAi							*ai;

	void						print(void) const;
	bool						wincondition(void) const;
	int							captures(void) const;
	int							last_move(void) const;
	void						reset(void);

	// Player						&operator=(Player const &rhs);

private:
	t_player					*player;
};

#endif