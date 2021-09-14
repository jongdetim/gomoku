#include "Board.hpp"
#include "algorithm.hpp"
#include <iostream>
#include <cmath>

#define HEIGHT 5
#define SIZE (HEIGHT*HEIGHT)
#define HOR 0b1111
#define VER 0b1000010000100001
#define DIAG2 0b000010001000100
#define DIAG1 0b01000001000001000001

void    print_bitmap(std::bitset<SIZE> bitmap)
{
    for (int i = 0; i < bitmap.size(); i++)
    {
        if (i != 0 and i % HEIGHT == 0)
            std::cout << std::endl;
        std::cout << bitmap[i];
        std::cout << ' ';
    }
    std::cout << std::endl << std::endl;
}

int	count_bits(unsigned int number)
{
	return (int)log2(number)+1;
}

int	count_set_bits(unsigned int number)
{
	int count = 0;
	while (number)
	{
		count += number & 1;
		number >>= 1;
	}
	return count;
}

static bool alignment(std::bitset<SIZE> state, std::bitset<SIZE> pattern, int pattern_width, int pattern_height)
{
	assert(pattern_width <= HEIGHT and pattern_width <= HEIGHT);
	for (int i = 0; i < SIZE; i++)
	{
		if (((i / HEIGHT) + pattern_height) > HEIGHT)
			break ;
		print_bitmap(pattern<<i);
		// if((state & (pattern<<i)) == (pattern<<i))
		// {
		// 	print_bitmap(pattern<<i);
		// 	return true;
		// }
		if ((i + pattern_width) % HEIGHT == 0)
			i += (pattern_width - 1);
	}
	return false;
}

int main()
{
	// std::bitset<SIZE> bitmap_diag{0b1000001000001000001000000};
	// print_bitmap(bitmap_diag);
	// std::cout << "*********" << std::endl;
	// std::cout << alignment(bitmap_diag, DIAG1, 4, 4) << std::endl;
	// std::cout << "---------" << std::endl;

	// std::bitset<SIZE> bitmap_diag2{0b0001000100010000000000000};
	// print_bitmap(bitmap_diag2);
	// std::cout << "*********" << std::endl;
	// std::cout << alignment(bitmap_diag2, DIAG2, 3, 3) << std::endl;
	// std::cout << "---------" << std::endl;


	// std::bitset<SIZE> bitmap_hor{0b0000000000011110000000000};
	// print_bitmap(bitmap_hor);
	// std::cout << "*********" << std::endl;
	// std::cout << alignment(bitmap_hor, HOR, 4, 1) << std::endl;
	// std::cout << "---------" << std::endl;

	std::bitset<SIZE> bitmap_ver{0b0000001000010000100001000};
	print_bitmap(bitmap_ver);
	std::cout << "*********" << std::endl;
	std::cout << alignment(bitmap_ver, VER, 1, 4) << std::endl;
	std::cout << "---------" << std::endl;

  return 0;
}
