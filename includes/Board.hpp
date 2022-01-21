#ifndef BOARD_HPP
# define BOARD_HPP

# include "gomoku.hpp"
# include "Player.hpp"
# include "Heuristic.hpp"
# include "IGameEngine.hpp"

# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define DEFAULT "\033[0m"

# define PLAYER this->current_player
# define PLAYER1 this->player1
# define PLAYER2 this->player2
# define P1_SYMBOL 'o'
# define P2_SYMBOL 'x'

class	Board
{
public:
	Board(void);
	Board(player_fn p1_fn);
	Board(player_fn p1_fn, player_fn p2_fn);
	~Board();
	
	int						h;
	std::bitset<BOARDSIZE>	filled_pos;
	Player					player1;
	Player					player2;
	Player					*current_player;
	int						winner;
	
	void					play(IGameEngine &engine);
	void					print(void) const;
	void					show_move(void) const;
	void					show_move(int show_index) const;
	bool					place(int index);
	bool					place(int index, int player_id);
	bool					place(int index, Player &player);
	bool					place(int row, int col, int player_id);
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	int						check_captures(Player &player_id, int index);
	std::vector<Board>		generate_children(void) const;
	bool					check_free_threes(int move, int player) const;
	int						calc_heuristic(void);
	int						calc_heuristic(Board &node);
	void					next_player(void);
	void					switch_to_player(int id);
	int						total_stones_in_play(void) const;
	void					random_player(void);

	int						get_player_id(int index) const;
	BITBOARD				get_state(void) const;
	int						get_last_move(void) const;
	Player					*get_player_by_id(int id);
	Board					get_copy(void) const;

	bool					is_game_finished(Player &player);
	bool					check_win_other_player(Player &player) const;
	bool					has_won(void) const;
	bool					has_won(Player &player) const;
	bool					is_empty_place(int index) const;
	bool					is_full(void) const;
	bool					is_valid_move(int index, Player &player) const;
	
	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;

private:
	BITBOARD				state;
	int						last_move;
	Heuristic				heuristic;

	bool					still_winning(Player &player) const;
	bool					can_capture(Player &player, int index, int dir) const;
	void					capture(int dir, int index);
	std::bitset<BOARDSIZE>	get_moves(void) const;
	bool					free_threes_direction(int move, int direction, int player) const;
};

std::ostream &operator<<(std::ostream &o, Board const &i);

#endif