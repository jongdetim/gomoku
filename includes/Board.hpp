#ifndef BOARD_HPP
# define BOARD_HPP

# include <bitset>

# define BOARDSIZE 19
# define MASKSIZE ((BOARDSIZE*BOARDSIZE)<<2)
# define MASK_LENGTH (BOARDSIZE<<1)
# define PLAYER1 -1
# define PLAYER2 1

class Board
{
public:
	Board(void);
	~Board();

	void					print(void);
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	bool					is_empty_place(int index);
	std::bitset<MASKSIZE>	get_state(void) const;

	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;
	std::bitset<MASKSIZE>	operator&(Board const &rhs) const;
	std::bitset<MASKSIZE>	operator&(std::bitset<MASKSIZE> const &rhs) const;

private:
	std::bitset<MASKSIZE>	state;
};

std::ostream &operator<<(std::ostream &o, Board const &i);

#endif