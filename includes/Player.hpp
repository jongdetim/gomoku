#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"
# include "IAi.hpp"

class Board;

typedef struct					s_player
{
	int							last_move		= -1;
	int							captures		= 0;
	int							score			= 0;
	uint8_t						patterns[8]		= {0};
	bool						wincondition	= false;
}								t_player;

// class GuiPlayer
// {
// public:

// 	GuiPlayer(t_player &player);
// 	GuiPlayer(const Player &rhs);

// 	std::string					name;
// 	t_player					*player;
// 	IAi							*ai;

// 	void						print(void) const;
// 	void						reset(void);
// 	int							get_next_player_index(void) const;

// 	Player						&operator=(Player const &rhs);

// private:
// };

#endif