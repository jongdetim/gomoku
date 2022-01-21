#ifndef IAI_HPP
# define IAI_HPP

class Board;

class IAi
{
public:
	virtual int	calculate(Board &board) = 0;
};

#endif
