/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhignett <fhignett@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/10 13:39:39 by fhignett      #+#    #+#                 */
/*   Updated: 2021/09/10 17:26:51 by fhignett      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Board.hpp"
#include <iostream>
#include <cassert>

Board::Board(void)
{
	this->board.reset();
}

Board::~Board() {}

void			Board::print(void)
{
	std::cout << "    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARDSIZE; row++)
	{
		if (row < 10)
			std::cout << row << " : ";
		else
			std::cout << row << ": ";
		for (int col = 0; col < BOARDSIZE; col++)
		{
			int index = (row * BOARDSIZE + col) * 2;
			if (this->is_empty(index))
				std::cout << ". ";
			else if (this->board[index])
				std::cout << PLAYER1 << ' ';
			else
				std::cout << PLAYER2 << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << this->board << std::endl;
}

bool			Board::place(int row, int col, int player)
{
	assert(player == 1 or player == 2);
	assert(row < BOARDSIZE and row >= 0 and col < BOARDSIZE and col >= 0);
	int index = (row * BOARDSIZE + col) * 2;
	if (not this->is_empty(index))
		return false;
	index += (player - 1);
	this->board[index] = true;
	return true;
}

inline bool		Board::is_empty(int index)
{
	return (this->board[index] == false and this->board[index+1] == false);
}
