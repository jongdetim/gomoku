#ifndef IGAMEENGINE_HPP
# define IGAMEENGINE_HPP

# include "IAi.hpp"

enum action
{
	def = 0,
	restart = 1,
	quit = 2,
	pause = 4
};

class Board;

class IGameEngine
{
public:
	IGameEngine(IAi *ai) : ai(ai) {}

	virtual void	play(Board *board) = 0;
	virtual void	reset(Board &board) = 0;
	virtual void	gameloop(Board &board) = 0;

	IAi				*ai;
};

#endif
