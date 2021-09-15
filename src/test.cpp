#include "Board.hpp"
#include "heuristic.hpp"
#include "misc.hpp"
#include <cassert>

void	test_board_class(void)
{
	Board board;

	board.place(0, PLAYER1);
	assert(board == 0b1);

	board.place(1, PLAYER2);
	assert(board == 0b1001);

	board.place(0, 1, PLAYER1);
	assert(board == 0b1001);

	board.place(0, 2, PLAYER1);
	assert(board == 0b11001);

	board.remove(0);
	assert(board == 0b11000);

	assert(board.is_empty_place(0) == true);
	assert(board.is_empty_place(2) == false);
}

void	pattern_test(void)
{
	t_size					size_pattern;
	std::bitset<MASKSIZE>	pattern;
	int						players[2]{-1,1};
	int						sizes[3]{5,4,3};

	Board board = create_random_board();
	std::cout << "************Starting board************" << std::endl;
	board.print();
	std::cout << "************Starting board************" << std::endl << std::endl;

	for (auto size : sizes)
	{
		for (auto player : players)
		{
			pattern = create_ver(size, size_pattern);
			std::cout << find_pattern(board, player, pattern, size_pattern) << " found for player: " << player << " (ver)size:"<< size << std::endl;

			pattern = create_hor(size, size_pattern);
			std::cout << find_pattern(board, player, pattern, size_pattern) << " found for player: " << player << " (hor)size:"<< size << std::endl;

			pattern = create_diag_left(size, size_pattern);
			std::cout << find_pattern(board, player, pattern, size_pattern) << " found for player: " << player << " (diagl)size:"<< size << std::endl;

			pattern = create_diag_right(size, size_pattern);
			std::cout << find_pattern(board, player, pattern, size_pattern) << " found for player: " << player << " (diagr)size:"<< size << std::endl;
		}
	}
	std::cout << std::endl <<"************Starting board************" << std::endl;
	board.print();
	std::cout << "************Starting board************" << std::endl;
}
