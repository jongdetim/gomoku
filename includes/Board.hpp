/* MIT License

Copyright (c) 2022 Flint Louis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#ifndef BOARD_HPP
# define BOARD_HPP

#include "gomoku.hpp"

# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define DEFAULT "\033[0m"

# define INDEX (index << 1)

# define PLAYER this->players[this->current]
# define P1_SYMBOL 'o'
# define P2_SYMBOL 'x'

class TranspositionTable;
class IGameEngine;
class TableEntry;

void init_zobrist_map();

typedef struct					s_player
{
	int							last_move		= -1;
	int							captures		= 0;
	uint8_t						patterns[8]		= {0};
	bool						wincondition	= false;
}								t_player;

class	Board
{
public:
	Board(void);
	Board(const Board &rhs);
	~Board();
	
	int						h;
	std::bitset<BOARDSIZE>	filled_pos;
	t_player				players[2];
	int						winner;
	bool					last_move_was_capture;
	
	void					print(void) const;
	void					print_values(void) const;
	void					show_last_move(void) const;
	void					show_move(int show_index) const;
	bool					place(int index);
	bool					place(int index, int player);
	bool					place(int row, int col, int player);
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	
	void					save(std::string file_name) const;
	void					load(std::string file_name);

	std::vector<Board>		generate_children(int player) const;

	int						check_captures(int player, int index);
	int						check_wincondition_all_dir(int index, int player) const;
	bool					is_capture(int player, int index) const;

	int						calculate_index(int row, int col) const;
	int						calc_heuristic(void);
	int						calc_heuristic(Board &node);
	void					print_principal_variation(int player, int depth, TranspositionTable &t_table);
	void					print_player_patterns(int player) const;
	void					print_players_patterns(void) const;

	void					set_current_player(int player);
	void					next_player(void);
	int						get_next_player(int player) const;
	int						get_next_player(void) const;
	int						get_player(int index) const;
	BITBOARD				get_state(void) const;
	int						get_last_move(void) const;
	int						get_last_player(void) const;
	int						get_current_player(void) const;

	bool					is_free_threes(int move, int player) const;
	bool					is_player_on_index(int index, int player) const;
	bool					is_empty_place(int index) const;
	bool					is_valid_move(int index) const;
	bool					is_full(void) const;
	bool					is_empty(void) const;
	bool					has_winner(void) const;
	bool					is_game_finished(void);
	bool					is_game_finished(int player);
	bool					check_win_other_player(int player);
	
	Board					&operator=(Board const &rhs);
	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;

	void					update_hash(int index, int player);
	static void				init_zobrist_map();

	uint64_t				zobrist_hash;
	static uint64_t			zobrist_map[MASKSIZE];

private:
	BITBOARD				state;
	int						current_player;
	int						last_move;

	int						total_stones_in_play(void) const;
	int						check_wincondition_all_dir(int player) const;
	bool					continue_game(int player);
	bool					has_won(int player);
	bool					can_capture(int player, int index, int dir) const;
	void					capture(int dir, int index);
	std::bitset<BOARDSIZE>	get_moves(void) const;
	bool					free_threes_direction(int move, int direction, int player) const;
	bool					still_winning(int player) const;
};

std::ostream &operator<<(std::ostream &o, Board const &i);

#endif