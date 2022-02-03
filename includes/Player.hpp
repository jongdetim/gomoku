#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"
# include "IAi.hpp"

class Board;

typedef struct					s_player
{
	int							captures = 0;
	int							score = 0;
	uint8_t						patterns[8] = {0};

}								t_player;

class GuiPlayer
{
public:

	GuiPlayer(void);
	GuiPlayer(const Player &rhs);
	GuiPlayer(int player_id, std::string name);

	std::string					name;
	int							id;
	int							index;
	int							last_move;
	int							stones_in_play;
	int							winning_index;
	t_player					player;
	IAi							*ai;

	void						print(void) const;
	void						reset(void);
	bool						has_wincondition(void) const;
	int							get_next_player_index(void) const;

	Player						&operator=(Player const &rhs);
	bool						operator==(Player &rhs) const;
	bool						operator==(int id) const;
	bool						operator!=(Player &rhs) const;
	bool						operator!=(int id) const;

private:
};

#endif