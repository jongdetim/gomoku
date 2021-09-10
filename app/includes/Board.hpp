/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhignett <fhignett@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/10 16:11:37 by fhignett      #+#    #+#                 */
/*   Updated: 2021/09/10 17:00:23 by fhignett      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_HPP
# define BOARD_HPP

# include <bitset>

# define MASKSIZE (19*19*2)
# define BOARDSIZE 19
# define PLAYER1 'o'
# define PLAYER2 'x'

class Board
{
public:
	Board(void);
	~Board();

	void				print(void);
	bool				place(int row, int col, int player);

private:
	inline bool			is_empty(int index);
	std::bitset<MASKSIZE>	board;
};

#endif