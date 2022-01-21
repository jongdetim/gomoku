#ifndef PRIMITIVEGUI_HPP
# define PRIMITIVEGUI_HPP

# include "Board.hpp"
# include "misc.hpp"

class PrimitiveGui : public IGameEngine
{
public:
	void	play(Board *board);

private:
	void						gameloop(Board &board);
	void						reset(Board &board);
	void						print_stats(Board &board) const;
	std::vector<std::string>	tokenize(std::string &str, char delim);
	bool						try_parse_input(std::string &input, int &out);
	int							get_player_input(void);
	int							get_index(Board &board);
	void						print_winner(Board &board) const;
};

#endif
