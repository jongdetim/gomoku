#ifndef IGAMEENGINE_HPP
# define IGAMEENGINE_HPP

class NegamaxAi;

enum e_action 
{
	def = 0,
	restart = 1,
	quit = (1<<1),
	pauze = (1<<2),
	undo = (1<<3)
};

class Board;

class IGameEngine
{
public:
	IGameEngine(NegamaxAi *ai) : ai(ai) {}

	virtual void	play(Board board) = 0;
	virtual void	reset(void) = 0;
	virtual void	gameloop(void) = 0;

	NegamaxAi		*ai;
};

#endif
