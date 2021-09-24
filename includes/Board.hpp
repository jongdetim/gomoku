#ifndef BOARD_HPP
# define BOARD_HPP

#include <iostream>
#include <cassert>
#include <bitset>
#include <vector>
#include <unordered_set>

# define BOARDSIZE 19
# define MASKSIZE ((BOARDSIZE*BOARDSIZE)<<1)
# define MASK_LENGTH (BOARDSIZE<<1)
# define PLAYER1 1
# define PLAYER2 -1

// globals
const int NEIGHBOURS[8] = {-20, -19, -18, -1, 1, 18, 19, 20};

class Board
{
public:
	Board(void);
	~Board();
	void					print(void) const;
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	bool					is_game_finished() const;
	int						get_random_heuristic() const;
	std::vector<Board> 		generate_children(std::vector<int> filled_positions, int player) const;
	std::unordered_set<int>	get_moves(std::vector<int> filled_positions) const;
	std::bitset<MASKSIZE>	get_state(void) const;
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	bool					is_empty_place(int index) const;
	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;
	std::bitset<MASKSIZE>	operator&(Board const &rhs) const;
	std::bitset<MASKSIZE>	operator&(std::bitset<MASKSIZE> const &rhs) const;

	int						last_move;
	int						h;

private:
	std::bitset<MASKSIZE>	state;
};

std::ostream &operator<<(std::ostream &o, Board const &i);

#endif