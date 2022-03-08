#ifndef PRIMITIVEGUI_HPP
# define PRIMITIVEGUI_HPP

# include "GuiBoard.hpp"
# include "IAi.hpp"
# include "IGameEngine.hpp"

class PrimitiveGui : public IGameEngine
{
public:
	PrimitiveGui(IAi *ai);
	
	void	play(Board board);

private:
	short						action;
	GuiBoard					guiboard;

	void						gameloop(void);
	void						reset(void);
	void						print_stats(void);
	bool						try_parse_input(std::string &input, int &out);
	int							get_player_input(void);
	int							get_index(void);
	void						print_winner(void);
};

#endif
