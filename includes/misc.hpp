#ifndef MISC_HPP
# define MISC_HPP

# include <chrono>
# include <iostream>

# include "gomoku.hpp"

#define PRINT(x) std::cout << x << std::endl

extern int TOTAL_NODES;
extern int TOTAL_LEAVES;
extern int DEBUG_COUNTER;
extern int FOUND_IN_TABLE;
extern int TOTAL_BRANCHES_PRUNED;

class Board;

class Timer
{
public:
    void start();
    void stop();
    int elapsedMilliseconds();
    double elapsedSeconds();
private:
    std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
    std::chrono::time_point<std::chrono::steady_clock> m_EndTime;
    bool                                               m_bRunning = false;
};

namespace misc
{
	Board						create_random_board(void);
	Board						create_random_board(int seed);
	int							get_col(int index);
	int							get_row(int index);
	int							calc_index(int row, int col);
	bool						is_offside(int prev_index, int index);
	void						create_star(Board &board, int index, int size, int player_index);
	void						place_pieces(Board &board, int start_pos, int amount, int offset);
	int							get_random_int_range(int min, int max);
	int							get_random_int_range(int min, int max, int seed);
	int							get_random_int(void);
	std::vector<std::string>	tokenize(std::string &str, char delim);
	void                        print_and_quit(const char *msg);
	void	                    print_stats();
}

#endif
