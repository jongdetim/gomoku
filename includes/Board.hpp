/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhignett <fhignett@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/10 16:11:37 by fhignett      #+#    #+#                 */
/*   Updated: 2021/09/13 21:08:21 by tide-jon      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_HPP
# define BOARD_HPP

#include <iostream>
#include <cassert>
#include <bitset>
#include <vector>
#include <unordered_set>

# define MASKSIZE (19*19*2)
# define BOARDSIZE 19
# define PLAYER1 -1
# define PLAYER2 1

// globals
const int neighbours[8] = {-20, -19, -18, -1, 1, 18, 19, 20};

class Board
{
public:
	Board(void);
	~Board();
	void					print(void);
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	bool					is_game_finished();
	int						get_heuristic();
	std::vector<Board> 		generate_children(std::vector<int> filled_positions, int player);
	std::unordered_set<int>	get_moves(std::vector<int> filled_positions);

	int						last_move;

private:
	inline bool				is_empty_place(int index);
	std::bitset<MASKSIZE>	state;
};

#endif