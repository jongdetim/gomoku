# include "gomoku.hpp"
# include "Heuristic.hpp"
# include "Board.hpp"
# include "TranspositionTable.hpp"
# include "misc.hpp"
# include "algorithm.hpp"

#define PRINT(x) std::cout << x << std::endl

void               cutout_pattern(const Board &board, int move, int direction, int player, t_pattern &pat)
{
    int shift = DIRECTIONS[direction + 4];
    int pos = move - (pat.left_right[0] * shift);
    for (int i = 0; i < pat.length; i++)
    {
        pat.pattern += abs(board.get_player(pos));
        if (i != pat.length - 1)
            pat.pattern <<= 1;
        pos += shift;
    }
}

t_pattern			get_pattern(const Board &board, int move, int direction, int player)
{
	t_pattern pattern;
	int pos;
	pattern.space = 1;
	pattern.count = 1;
	int shift;
	for (int j = 0; j < 2; j++)
	{
		pos = move;
		shift = DIRECTIONS[direction + 4 * j];
		int i = 0;
		int open = 0;
		while (open < 2)
		{
			pos += shift;
            if (open == 0)
			{
                if (is_offside(pos - shift, pos) || board.get_player(pos) == -player)
                {
                    pattern.left_right[j] = i;
                    break;
                }
                if (board.get_player(pos) == player)
                    pattern.count++;
                else if (board.get_player(pos - shift) == player) //leeg vakje maar vorige niet
                    pattern.left_right[j] = i + 1;
                else
                    open++;
            }
			else if (!is_offside(pos - shift, pos) && board.get_player(pos) == (0 || player))
				open++;
            else
                break;
			pattern.space++;
			i++;
		}
	}
	pattern.length = pattern.left_right[0] + pattern.left_right[1] + 1;
    cutout_pattern(board, move, direction, player, pattern);
	return pattern;
}

Pattern search_subpattern(t_pattern &pat, t_pattern &sub)
{
    int8_t times = pat.length - sub.length;

    if (times < 0)
    {
        PRINT("subpattern does not fit inside of cutout pattern");
        exit(1);
    }
    for (int8_t i = 0; i < times + 1; i++)
    {
        PRINT(std::bitset<8>(uint8_t(pat.pattern << (8 - pat.length + i)) >> (times + (8 - pat.length))));
        if(uint8_t(pat.pattern << (8 - pat.length + i)) >> (times + (8 - pat.length)) == sub.pattern)
        {
            // std::cout << std::bitset<8>(uint8_t(pat.pattern << i) >> times) << std::endl;
            PRINT("PATTERN FOUND");
            // pattern found, do the thing
        }
    }
    return none;
}

int main()
{
    Board board;
    int index = calc_index(0, 0);
    // int star_index = calc_index(3, 16); 

    // create_star(board, star_index, 3, PLAYER1);
    // create_star(board, star_index, 4, PLAYER2);
    
    // board.remove(star_index);

    // board.place(star_index, PLAYER2);
    // std::cout << "captures : " << board.get_player_captures(PLAYER2) << std::endl;

    board.place(index, PLAYER1);
    // board.place(index + 3, PLAYER1);
    // board.place(index + 4, PLAYER1);
    // board.place(index + 9, PLAYER1);
    // board.place(index + 8, PLAYER1);
    // board.place(index + 6, PLAYER1);
    board.place(360, PLAYER1);
    board.show_last_move();
    // exit(1);

    // get the pattern
    t_pattern pat = get_pattern(board, board.get_last_move(), 0, PLAYER1);
    PRINT(pat.count);
    PRINT(pat.left_right[0]);
    PRINT(pat.left_right[1]);
    PRINT(pat.length);
    PRINT(pat.space);
    PRINT(std::bitset<8>(pat.pattern));


    // see if some sub-pattern is inside the cutout pattern
    t_pattern sub;
    sub.pattern = 0b00000001;
    sub.length = 1;
    search_subpattern(pat, sub);
    
    // if (board.is_game_won())
    //     std::cout << "Won!!!" << std::endl;
    // else
    //     std::cout << "Not Won :(" << std::endl;

    return 0;
}
