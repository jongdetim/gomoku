#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"
# include "Board.hpp"

class NegamaxAi;

class GuiPlayer
{
public:

	GuiPlayer(void);
	GuiPlayer(t_player &player);

	std::string					name;
	NegamaxAi					*ai;

	void						print(void) const;
	bool						wincondition(void) const;
	int							captures(void) const;
	int							last_move(void) const;
	void						reset(void);

private:
	t_player					*player;
};

#endif