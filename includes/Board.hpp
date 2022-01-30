#ifndef BOARD_HPP
# define BOARD_HPP

# include "gomoku.hpp"
# include "heuristic.hpp"
// # include "TranspositionTable.hpp"

class TranspositionTable;

class TableEntry
{
public:
    int value;
    int depth;
    int flag;
    bool game_finished;
    int best_move;
};

typedef struct	s_player
{
	int			captures;
	Heuristic	heuristic;
}				t_player;

class Board
{
public:
	Board(void);
	~Board();
	
	t_player				players[2];
	int						h;
	std::bitset<BOARDSIZE>	filled_pos;
	// this Heuristic class is only to easily access class functions. made them static instead
	// Heuristic				heuristic;
	// std::bitset<BOARDSIZE>	checked_indices[4] = {0, 0, 0, 0};
	
	void					print(void) const;
	void					show_last_move(void) const;
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	void					set_state(BITBOARD new_state);
	int						calculate_index(int row, int col) const;
	int						check_captures(int player, int index);
	std::vector<Board>		generate_children(int player) const;
	bool					check_free_threes(int move, int player) const;
	int						calc_heuristic(void);
	int						calc_heuristic(Board &node);
	void					print_principal_variation(int player, int depth, TranspositionTable &t_table);

	int						get_player(int index) const;
	int						get_last_player(void) const;
	int						get_stones_in_play(void) const;
	int						get_player_captures(int player) const;
	BITBOARD				get_state(void) const;
	int						get_last_move(void) const;
	
	bool					is_empty_place(int index) const;
	bool					is_game_finished() const;
	bool					is_full(void) const;
	bool					is_game_won(void) const;
	bool					is_game_won(int index, int player) const;
	bool					is_valid_move(int index, int player) const;
	
	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;
	BITBOARD				operator&(Board const &rhs) const;
	BITBOARD				operator&(BITBOARD const &rhs) const;
	// Board&					operator=(Board const &rhs);

private:
	BITBOARD				state;
	int						last_move;
	int						stones_in_play;

	int						get_player_index(int index, int player) const;
	bool					can_capture(int player, int index, int dir) const;
	void					capture(int dir, int index);
	void					update_player(int player, int captures);
	std::bitset<BOARDSIZE>	get_moves(void) const;
	bool					free_threes_direction(int move, int direction, int player) const;

};
// wat doet dit..?
std::ostream &operator<<(std::ostream &o, Board const &i);

#endif