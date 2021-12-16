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
                    pattern.space++;
                    break;
                }
                if (board.get_player(pos) == player)
                    pattern.count++;
                else if (board.get_player(pos - shift) == player) //leeg vakje maar vorige niet
                    pattern.left_right[j] = i + 1;
                else
                    open++;
            }
			else if (is_offside(pos - shift, pos) || board.get_player(pos) == -player)
                break;
            else
				open++; 
			pattern.space++;
			i++;
		}
	}
	pattern.length = pattern.left_right[0] + pattern.left_right[1] + 1;
    cutout_pattern(board, move, direction, player, pattern);
	return pattern;
}

// bool search_subpattern(t_pattern &pat, t_pattern &sub)
// {
//     int8_t times = pat.length - sub.length;

//     if (times < 0)
//     {
//         PRINT("subpattern does not fit inside of cutout pattern");
//         exit(1);
//     }
//     for (int8_t i = 0; i < times + 1; i++)
//     {
//         PRINT(std::bitset<8>(uint8_t(pat.pattern << (8 - pat.length + i)) >> (times + (8 - pat.length))));
//         if(uint8_t(pat.pattern << (8 - pat.length + i)) >> (times + (8 - pat.length)) == sub.pattern)
//         {
//             // std::cout << std::bitset<8>(uint8_t(pat.pattern << i) >> times) << std::endl;
//             PRINT("PATTERN FOUND");
//             return true;
//         }
//     }
//     return false;
// }

void    print_and_quit(const char *msg)
{
    PRINT(msg);
    exit(1);
}

Pattern find_subpattern(t_pattern &pat, uint8_t length, std::map<uint8_t, Pattern> map)
{
    int8_t fit = pat.length - length;
    Pattern result = none;

    if (fit < 0)
        print_and_quit("cutout pattern is too smol");
    for (int8_t i = 0; i < fit + 1; i++)
    {
        auto iter = map.find(uint8_t(pat.pattern << (8 - pat.length + i)) >> (fit + (8 - pat.length)));
        if (iter != map.end() && iter->second > result)
        {
            result = iter->second;
            if (result == five || result == open4)
                return result;
        }
    }
    return result;
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

    board.place(index + 1, PLAYER1);
    board.place(index, PLAYER1);
    board.place(index + 4, PLAYER1);
    board.place(index + 8, PLAYER1);
    board.place(index + 6, PLAYER1);
    // board.place(360, PLAYER1);
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
    // t_pattern sub;
    // sub.pattern = 0b00000001;
    // sub.length = 3;
    Pattern result;
    PRINT(PatternNames[result = find_subpattern(pat, 5, SUBPATTERNS_5)]);
    if (result != five)
        PRINT(PatternNames[result = find_subpattern(pat, 6, SUBPATTERNS_6)]);
    
    // if (board.is_game_won())
    //     std::cout << "Won!!!" << std::endl;
    // else
    //     std::cout << "Not Won :(" << std::endl;

    return 0;
}
