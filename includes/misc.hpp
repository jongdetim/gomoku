/* MIT License

Copyright (c) 2022 Flint Louis, Tim de Jong

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
	int elapsed_ms = 0;

	Timer(int timeout);

    void	start();
    void	stop();
    int		elapsedMilliseconds();
    double	elapsedSeconds();
	bool	timeout_reached(void);
private:
	int	timeout;

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
