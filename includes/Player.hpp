#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"

class Player;

typedef int (*player_fn)(void);

class Player
{
public:

	Player(int player_id, std::string name);

	std::string					name;
	int							id;
	int							index_offset;
	int							last_move;
	int							captures;
	int							stones_in_play;
	int							winning_index;
	Player						*next;
	player_fn					fn;

	void						set_fn(player_fn fn);
	void						print(void) const;
	void						reset(void);
	bool						has_wincondition(void) const;
	int							index(int index) const;

	bool						operator==(Player &rhs) const;
	bool						operator==(int id) const;
	bool						operator!=(Player &rhs) const;
	bool						operator!=(int id) const;

private:
};

#endif