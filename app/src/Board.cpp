/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhignett <fhignett@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/10 13:39:39 by fhignett      #+#    #+#                 */
/*   Updated: 2021/09/12 17:46:05 by flintlouis    ########   odam.nl         */
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
	std::cout << "   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8" << std::endl;
	for (int row = 0; row < BOARDSIZE; row++)
	{
		std::cout << row%10 << ": ";
		for (int col = 0; col < BOARDSIZE; col++)
		{
			int index = (row * BOARDSIZE + col) * 2;
			if (this->is_empty(index))
				std::cout << ". ";
			else if (this->board[index])
				std::cout << 'o' << ' ';
			else
				std::cout << 'x' << ' ';
		}
		std::cout << std::endl;
	}
}

bool			Board::place(int row, int col, int player)
{
	assert(player == PLAYER1 or player == PLAYER2);
	assert(row < BOARDSIZE and row >= 0 and col < BOARDSIZE and col >= 0);
	int index = (row * BOARDSIZE + col) * 2;
	if (not this->is_empty(index))
		return false;
	index += player;
	this->board[index] = true;
	return true;
}

bool			Board::place(int index, int player)
{
	assert(player == PLAYER1 or player == PLAYER2);
	assert(index >= 0 and index < (BOARDSIZE*BOARDSIZE));
	if (not this->is_empty(index))
		return false;
	index += player;
	this->board[index] = true;
	return true;
}

inline bool		Board::is_empty(int index)
{
	return (this->board[index] == false and this->board[index+1] == false);
}
