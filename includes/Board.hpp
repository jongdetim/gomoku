#ifndef BOARD_HPP
# define BOARD_HPP

# include <iostream>
# include <cassert>
# include <bitset>
# include <vector>
# include <unordered_set>
# include <set>
# include <algorithm>
# include "BoardHeuristic.hpp"

# define BOARD_LENGTH 19
# define BOARDSIZE (BOARD_LENGTH*BOARD_LENGTH)
# define MASKSIZE ((BOARD_LENGTH*BOARD_LENGTH)<<1)
# define MASK_LENGTH (BOARD_LENGTH<<1)
# define PLAYER1 1
# define PLAYER2 -1
# define BITBOARD std::bitset<MASKSIZE>

typedef struct	s_player
{
	int			captures;
}				t_player;

// globals
const int DIRECTIONS[8] = {DIAGUPL, UP, DIAGUPR, LEFT, RIGHT, DIAGDWNL, DOWN, DIAGDWNR};

class Board
{
public:
	Board(void);
	~Board();

	int						h;
	std::bitset<BOARDSIZE>	filled_pos;
	
	void					print(void) const;
	void					show_last_move(void) const;
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	bool					is_game_finished() const;
	std::vector<Board>		generate_children(int player) const;
	BITBOARD				get_state(void) const;
	int						get_last_move(void) const;
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	bool					is_empty_place(int index) const;
	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;
	BITBOARD				operator&(Board const &rhs) const;
	BITBOARD				operator&(BITBOARD const &rhs) const;
	void					set_state(BITBOARD new_state);
	int						get_player(int index) const;
	int						get_last_player(void) const;
	bool					is_full(void) const;
	bool					is_game_won(void) const;
	int						calculate_index(int row, int col) const;
	int						get_captures(int player) const;
	int						get_stones_played(void) const;
	int						get_player_index(int index, int player) const;

private:
	BITBOARD				state;
	int						last_move;
	BoardHeuristic			heuristic;
	t_player				player1;
	t_player				player2;
	int						stones_played;

	int						check_captures(int player);
	void					capture(int dir, int index);
	void					update_player(int player, int captures);
	bool					is_valid_move(int index, int player) const;
	std::bitset<BOARDSIZE>	get_moves(void) const;
};

std::ostream &operator<<(std::ostream &o, Board const &i);

#endif