#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"

class Board;

typedef int (*player_fn)(Board&);

class Player
{
public:

	Player(int player_id, std::string name, player_fn fn);

	std::string					name;
	int							id;
	int							last_move;
	int							captures;
	int							stones_in_play;
	int							winning_index;
	Player						*next;
	player_fn					fn;

	void						print(void) const;
	void						reset(void);
	bool						has_wincondition(void) const;
	bool						has_function(void) const;
	int							index(int index) const;

	bool						operator==(Player &rhs) const;
	bool						operator==(int id) const;
	bool						operator!=(Player &rhs) const;
	bool						operator!=(int id) const;

private:
	int							index_offset;
};

int								get_player_input(Board &_);

#endif