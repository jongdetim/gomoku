#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"

class Board;

class Player
{
public:

	Player(void);
	Player(const Player &rhs);
	Player(int player_id, std::string name);

	std::string					name;
	int							id;
	int							last_move;
	int							captures;
	int							stones_in_play;
	int							winning_index;
	Player						*next;

	void						print(void) const;
	void						reset(void);
	bool						has_wincondition(void) const;
	int							index(int index) const;

	Player						&operator=(Player const &rhs);
	bool						operator==(Player &rhs) const;
	bool						operator==(int id) const;
	bool						operator!=(Player &rhs) const;
	bool						operator!=(int id) const;

private:

	int							index_offset;
};

#endif