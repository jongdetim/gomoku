#ifndef BOARD_HPP
# define BOARD_HPP

# include "gomoku.hpp"
# include "heuristic.hpp"
# include "IGameEngine.hpp"
# include "TranspositionTable.hpp"

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

# define PLAYERS this->players
# define PLAYER this->players[this->current]
# define P1_SYMBOL 'o'
# define P2_SYMBOL 'x'

class TranspositionTable;

class TableEntry
{
public:
    int						value;
    int						depth;
    int						flag;
    bool					game_finished;
    int						best_move;
};

typedef struct					s_player
{
	int							last_move		= -1;
	int							captures		= 0;
	int							score			= 0;
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
	
	void					play(IGameEngine &engine);
	void					print(void) const;
	void					show_last_move(void) const;
	void					show_move(int show_index) const;
	bool					place(int index);
	bool					place(int index, int player);
	bool					place(int row, int col, int player);
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	bool					is_empty_place(int index) const;
	bool					is_valid_move(int index) const;
	bool					is_full(void) const;
	int						total_stones_in_play(void) const;
	
	int						check_captures(int player, int index);

	std::vector<Board>		generate_children(void) const;
	std::vector<Board>		generate_children(int player) const;

	bool					check_free_threes(int move, int player) const;

	void					set_state(BITBOARD new_state);
	int						calculate_index(int row, int col) const;
	int						calc_heuristic(void);
	int						calc_heuristic(Board &node);
	void					print_principal_variation(int player, int depth, TranspositionTable &t_table);

	void					set_current_player(int player);
	bool					player_on_index(int index, int player) const;
	void					next_player(void);
	int						get_next_player(int player) const;
	int						get_next_player(void) const;
	void					random_player(void);
	int						get_player(int index) const;
	BITBOARD				get_state(void) const;
	int						get_last_move(void) const;
	int						get_last_player(void) const;
	int						get_current_player(void) const;

	bool					has_winner(void) const;
	bool					is_game_finished(void);
	bool					is_game_finished(int player);
	bool					check_win_other_player(int player);
	
	Board					&operator=(Board const &rhs);
	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;

private:
	BITBOARD				state;
	int						current_player;
	int						last_move;

	int						check_wincondition_all_dir(int index, int player) const;
	int						check_wincondition_all_dir(int player) const;
	bool					continue_game(int player);
	bool					has_won(int player);
	bool					can_capture(int player, int index, int dir) const;
	void					capture(int dir, int index);
	std::bitset<BOARDSIZE>	get_moves(void) const;
	bool					free_threes_direction(int move, int direction, int player) const;
	bool					still_winning(int player) const;
};
// wat doet dit..?
std::ostream &operator<<(std::ostream &o, Board const &i);

#endif