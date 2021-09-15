/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Board.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fhignett <fhignett@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/09/10 16:11:37 by fhignett      #+#    #+#                 */
/*   Updated: 2021/09/15 14:06:56 by flintlouis    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_HPP
# define BOARD_HPP

# include <bitset>

# define MASKSIZE (19*19*2)
# define BOARDSIZE 19
# define PLAYER1 0
# define PLAYER2 1

class Board
{
public:
	Board(void);
	~Board();

	void					print(void);
	bool					place(int row, int col, int player);
	bool					place(int index, int player);
	void					reset(void);

	bool					operator==(Board const &rhs) const;
	bool					operator!=(Board const &rhs) const;
	bool					operator==(int const rhs) const;
	bool					operator!=(int const rhs) const;
	std::bitset<MASKSIZE>	operator&(Board const &rhs) const;

private:
	inline bool				is_empty_place(int index);
	std::bitset<MASKSIZE>	state;
};

#endif