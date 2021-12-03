#ifndef PLAYER_HPP
# define PLAYER_HPP

# include "gomoku.hpp"

class Player;

// typedef std::shared_ptr<Player> playerPtr;
typedef int (*player_fn)(void);

class Player
{
public:
	std::string					name;
	int							id;
	int							index_offset;
	int							last_move;
	int							captures;
	int							stones_in_play;
	Player						*next;
	player_fn					fn;

	void						set_fn(player_fn fn);

private:
	// int							get_player_input(void);
	// std::vector<std::string>	tokenize(std::string &str, char delim);
	// bool						try_parse_input(std::string &input, int &out);
};

#endif