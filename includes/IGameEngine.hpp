#ifndef IGAMEENGINE_HPP
# define IGAMEENGINE_HPP

class Board;

class IGameEngine
{
public:
	virtual void	play(Board *board) = 0;
	virtual void	reset(Board &board) = 0;
	virtual void	gameloop(Board &board) = 0;
};

#endif
