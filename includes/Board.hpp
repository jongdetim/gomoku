#ifndef BOARD_HPP
# define BOARD_HPP

# include "gomoku.hpp"
# include "Heuristic.hpp"

# define BLACK "\033[0;30m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define PURPLE "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define DEFAULT "\033[0m"

typedef struct	s_player
{
	int			captures;
}				t_player;

class Board
{
public:
	Board(void);
	~Board();
	
	int						h;
	std::bitset<BOARDSIZE>	filled_pos;
	Heuristic				heuristic;
	
	void					play(void);
	void					print(void) const;
	void					show_last_move(void) const;
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	void					remove(int row, int col);
	void					remove(int index);
	void					reset(void);
	void					set_state(BITBOARD new_state);
	int						check_captures(int player, int index);
	std::vector<Board>		generate_children(int player) const;
	bool					check_free_threes(int move, int player) const;
	int						calc_heuristic(void);
	int						calc_heuristic(Board &node);

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

private:
	BITBOARD				state;
	int						last_move;
	t_player				player1;
	t_player				player2;
	int						stones_in_play;

	std::vector<std::string>tokenize(std::string &str, char delim);
	bool					try_parse_input(std::string &input, int &out);
	int						get_player_index(int index, int player) const;
	bool					can_capture(int player, int index, int dir) const;
	void					capture(int dir, int index);
	void					update_player(int player, int captures);
	std::bitset<BOARDSIZE>	get_moves(void) const;
	bool					free_threes_direction(int move, int direction, int player) const;


};

std::ostream &operator<<(std::ostream &o, Board const &i);

#endif